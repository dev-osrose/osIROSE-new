#include "gtest/gtest.h"

#include "crash_report.h"

#include <chrono>
#include <filesystem>
#include <string>
#include <thread>

#ifndef _WIN32
#include <sys/wait.h>
#include <unistd.h>
#endif

// What is worth pinning here is *not* that crashpad can write a minidump -
// that is crashpad's own test suite's job. It is that our wiring around it is
// live. The breakpad code this replaced sat in the tree for years compiling
// down to an empty stub, because ENABLE_CRASH_REPORTS was never defined by any
// CMakeLists, and nothing ever noticed. These tests fail loudly in that state
// instead.
//
// Everything that needs a real handler is gated on ENABLE_CRASH_REPORTS and
// reports as skipped rather than passing vacuously, so a run built with
// -DWITH_CRASH_REPORTS=OFF (the default, and what CI uses) tells the truth
// about what it did and did not check.

namespace {

  // Handler discovery defaults to "next to my own executable". The test binary
  // is built into the same bin/ directory as crashpad_handler, so the default
  // path is exercised here exactly as the servers exercise it - which is the
  // deployment assumption worth pinning.
  std::filesystem::path scratch_dir(const std::string& name) {
    const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
    const auto dir = std::filesystem::temp_directory_path() /
                     ("osirose_crash_test_" + name + "_" + std::to_string(stamp));
    std::filesystem::remove_all(dir);
    return dir;
  }

  bool has_minidump(const std::filesystem::path& database) {
    const auto pending = database / "pending";
    std::error_code ec;
    if (!std::filesystem::is_directory(pending, ec)) return false;
    for (const auto& entry : std::filesystem::directory_iterator(pending, ec)) {
      if (entry.path().extension() == ".dmp") return true;
    }
    return false;
  }

  // The handler is a separate process and finishes writing after the crashing
  // process is already reaped, so the dump cannot be asserted synchronously.
  bool wait_for_minidump(const std::filesystem::path& database,
                         std::chrono::seconds timeout = std::chrono::seconds(20)) {
    const auto deadline = std::chrono::steady_clock::now() + timeout;
    while (std::chrono::steady_clock::now() < deadline) {
      if (has_minidump(database)) return true;
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return false;
  }

} // namespace

// A bad handler path has to fail closed. This is the one case that is
// meaningful in both build configurations, so it is never skipped.
TEST(CrashReport, MissingHandlerLeavesReporterInactive) {
  const auto dir = scratch_dir("missing_handler");
  Core::CrashReport reporter(dir.string(), "TestServer", "",
                             (dir / "definitely_not_a_handler").string());
  EXPECT_FALSE(reporter.is_active());
  std::filesystem::remove_all(dir);
}

TEST(CrashReport, StartsHandlerAndInitialisesDatabase) {
#ifndef ENABLE_CRASH_REPORTS
  GTEST_SKIP() << "built without -DWITH_CRASH_REPORTS=ON";
#else
  const auto dir = scratch_dir("database");
  {
    Core::CrashReport reporter(dir.string(), "TestServer");
    ASSERT_TRUE(reporter.is_active())
        << "crashpad_handler should have been found next to the test binary";

    // crashpad writes its upload-enabled flag here; without it the handler
    // silently refuses to upload no matter what url it was given.
    EXPECT_TRUE(std::filesystem::exists(dir / "settings.dat"));
  }
  std::filesystem::remove_all(dir);
#endif
}

// The end-to-end check: a process that actually dies leaves a minidump behind.
TEST(CrashReport, CrashInChildProducesMinidump) {
#ifndef ENABLE_CRASH_REPORTS
  GTEST_SKIP() << "built without -DWITH_CRASH_REPORTS=ON";
#elif defined(_WIN32)
  GTEST_SKIP() << "needs fork(); the crashing half would have to be a re-exec on Windows";
#else
  const auto dir = scratch_dir("minidump");
  ASSERT_TRUE(std::filesystem::create_directories(dir));

  const pid_t pid = fork();
  ASSERT_NE(pid, -1) << "fork failed";

  if (pid == 0) {
    // Child. Must never return into gtest, so every path ends in _exit() or
    // the deliberate crash. Exit codes are distinct so a setup failure here is
    // not mistaken for a missing dump below.
    Core::CrashReport reporter(dir.string(), "TestServer");
    if (!reporter.is_active()) _exit(42);
    Core::ICrashReport::crash();
    _exit(43); // unreachable unless crash() stopped crashing
  }

  int status = 0;
  ASSERT_EQ(waitpid(pid, &status, 0), pid);

  ASSERT_FALSE(WIFEXITED(status) && WEXITSTATUS(status) == 42)
      << "child could not start a crashpad handler";
  ASSERT_FALSE(WIFEXITED(status) && WEXITSTATUS(status) == 43)
      << "ICrashReport::crash() no longer crashes";
  EXPECT_TRUE(WIFSIGNALED(status)) << "child was expected to die from a signal";

  EXPECT_TRUE(wait_for_minidump(dir)) << "no minidump written to " << dir.string();

  std::filesystem::remove_all(dir);
#endif
}

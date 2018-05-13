// Unit test driver
#include <gtest/gtest.h>

#include "logconsole.h"
#include "network_thread_pool.h"
#include "crash_report.h"

using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestInfo;
using ::testing::UnitTest;

int main(int argc, char *argv[]) {
  InitGoogleTest(&argc, argv);
  
  Core::CrashReport crashHandler("/tmp/dumps");

  UnitTest &unit_test = *UnitTest::GetInstance();
  Core::CLog::SetLevel(spdlog::level::trace);

  // Some tests run all 3 servers and they do not like it when they have too many threads.
  // Use the max concurrent count that way each server has the same amount of time.
  Core::NetworkThreadPool::GetInstance( std::thread::hardware_concurrency() );

  int ret_val = RUN_ALL_TESTS();
  spdlog::drop_all();
  Core::NetworkThreadPool::DeleteInstance();

  int unexpectedly_failed_tests = 0;
  for (int i = 0; i < unit_test.total_test_case_count(); ++i) {
    const TestCase &test_case = *unit_test.GetTestCase(i);
    for (int j = 0; j < test_case.total_test_count(); ++j) {
      const TestInfo &test_info = *test_case.GetTestInfo(j);
      // Counts failed tests that were not meant to fail (those without
      // 'Fails' in the name).
      std::string name = test_info.name();
      if (test_info.result()->Failed() &&
          name.find("Fails") == std::string::npos) {
        unexpectedly_failed_tests++;
      }
    }
  }

  // Test that were meant to fail should not affect the test program outcome.
  if (unexpectedly_failed_tests == 0) ret_val = 0;

  return ret_val;
}

// Unit test driver
#include "gtest/gtest.h"
#include "network_thread_pool.h"
#include "logconsole.h"

using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestInfo;
using ::testing::UnitTest;

int main(int argc, char *argv[]) {
  InitGoogleTest(&argc, argv);

  UnitTest &unit_test = *UnitTest::GetInstance();

//  size_t q_size = 1048576;
//  spdlog::set_async_mode( q_size );

//  std::vector<spdlog::sink_ptr> sinks;
//  sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
//  sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>("tests", "txt", 23, 59));
//  auto combined_logger = std::make_shared<spdlog::logger>("console", begin(sinks), end(sinks));
//  spdlog::register_logger(combined_logger);

//  combined_logger->set_level(spdlog::level::warn);

  spdlog::set_pattern( "\033[1;32m" "[%H:%M:%S.%e %z] [%L] [thread %t]" "\033[1;37m" "%v" "\033[0m" );

  Core::NetworkThreadPool::GetInstance();

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

  // Core::NetworkThreadPool::DeleteInstance();

  return ret_val;
}

// Unit test driver
#include "gtest/gtest.h"
#include "network_thread_pool.h"

using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestInfo;
using ::testing::UnitTest;

int main(int argc, char *argv[]) {
  InitGoogleTest(&argc, argv);

  UnitTest &unit_test = *UnitTest::GetInstance();
  Core::NetworkThreadPool::GetInstance();

  int ret_val = RUN_ALL_TESTS();
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
  if (unexpectedly_failed_tests == 0)
    ret_val = 0;

  // Core::NetworkThreadPool::DeleteInstance();

  return ret_val;
}

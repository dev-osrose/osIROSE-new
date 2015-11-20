//Unit test driver
#ifdef _WIN32
#pragma warning (push, 1)
#pragma warning(disable: 6326 28182 4616)
#endif
#include "gtest/gtest.h"
#ifdef _WIN32
#pragma warning (pop)
#endif

using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestInfo;
using ::testing::UnitTest;

int main( int argc, char* argv[] )
{
	InitGoogleTest(&argc, argv);

	UnitTest& unit_test = *UnitTest::GetInstance();

	int ret_val = RUN_ALL_TESTS();
	int unexpectedly_failed_tests = 0;
	for (int i = 0; i < unit_test.total_test_case_count(); ++i)
	{
		const TestCase& test_case = *unit_test.GetTestCase(i);
		for (int j = 0; j < test_case.total_test_count(); ++j)
		{
			const TestInfo& test_info = *test_case.GetTestInfo(j);
			// Counts failed tests that were not meant to fail (those without
			// 'Fails' in the name).
			if (test_info.result()->Failed() && strcmp(test_info.name(), "Fails") != 0)
			{
				unexpectedly_failed_tests++;
			}
		}
	}

	// Test that were meant to fail should not affect the test program outcome.
	if (unexpectedly_failed_tests == 0)
		ret_val = 0;

	return ret_val;
}

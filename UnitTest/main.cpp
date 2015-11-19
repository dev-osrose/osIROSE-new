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

int main( int argc, char* argv[] )
{
	InitGoogleTest(&argc, argv);
	//TODO: actually use the arg's to enable and disable tests

	return RUN_ALL_TESTS();
}

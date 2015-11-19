//Unit test driver
#include "gtest/gtest.h"

using ::testing::InitGoogleTest;

int main( int argc, char* argv[] )
{
	InitGoogleTest(&argc, argv);
	//TODO: actually use the arg's to enable and disable tests

	return RUN_ALL_TESTS();
}

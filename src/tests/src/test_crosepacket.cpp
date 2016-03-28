#include "gtest/gtest.h"
#include "gtest/gtest-spi.h"
#include <string>

#include "crosepacket.h"

using namespace RoseCommon;

TEST( TestCRosePacket, TestConstructor )
{
	EXPECT_NO_FATAL_FAILURE(CRosePacket packet(0xAAAA));
}

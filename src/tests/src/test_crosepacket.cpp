#include "gtest/gtest.h"
#include "gtest/gtest-spi.h"
#include <string>

#include "crosepacket.h"
#include "packets.h"

using namespace RoseCommon;

TEST( TestCRosePacket, TestConstructor )
{
	EXPECT_NO_FATAL_FAILURE(CRosePacket packet(0xAAAA));
}

TEST(TestCRosePacket, TestSrvCharListReply) {
	/* SrvCharacterListReply pak; */
	/* pak.addCharacter("test", 0, 0, 0); */
	/* auto p = pak.getPacked(); */
	/* uint8_t *data = p.get(); */
	/* for (size_t i = 0; i < pak.size(); ++i) */
		/* std::cout << std::hex << (int)data[i] << " "; */
	/* std::cout << std::endl; */
}

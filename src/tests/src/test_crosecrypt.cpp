//Unit test driver
#include "gtest/gtest.h"

#include <stdint.h>
#include "ePacketType.h"
#include "crosecrypt.h"

//PacketCodec *g_Crypt = nullptr;

TEST( TestRoseCrypt, TestDefaultCrypt )
{
	EXPECT_NO_FATAL_FAILURE( PacketCodec g_Crypt );
}

TEST( TestRoseCrypt, TestSetSeed )
{
	PacketCodec g_Crypt;
	EXPECT_NO_FATAL_FAILURE(g_Crypt.changeSeed(0x289012));
}

TEST( TestRoseCrypt, TestEncryptData )
{
	PacketCodec g_Crypt;
	CPacket pak( ePacketType::PAKCS_CHAR_LIST_REQ, sizeof(pakChannelList_Req) );
	pak.pChannelListReq.lServerID = 0x77;

	EXPECT_NO_FATAL_FAILURE( g_Crypt.encodeServerPacket( pak.Data ) );

	EXPECT_NE( sizeof(pakChannelList_Req), pak.pChannelListReq.Size );
	EXPECT_NE( (uint16_t)ePacketType::PAKCS_CHAR_LIST_REQ, pak.pChannelListReq.Command );
	EXPECT_NE( 0x77, pak.pChannelListReq.lServerID );
}

//TEST( RoseCrypt, DecryptData )
//{
//	g_Crypt.Default();
//
//	CPacket pak( ePacketType::PAKCS_CHAR_LIST_REQ, sizeof(pakChannelList_Req) );
//	pak.pChannelListReq.lServerID = 0x77;
//
//	EXPECT_NO_FATAL_FAILURE( g_Crypt.ESSP( pak.Data ) );
//
//	EXPECT_NE( sizeof(pakChannelList_Req), pak.pChannelListReq.Size );
//	EXPECT_NE( (uint16_t)ePacketType::PAKCS_CHAR_LIST_REQ, pak.pChannelListReq.Command );
//	EXPECT_NE( 0x77, pak.pChannelListReq.lServerID );
//}

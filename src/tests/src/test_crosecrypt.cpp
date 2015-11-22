//Unit test driver
#include "gtest/gtest.h"

#include <stdint.h>
#include "ePacketType.h"
#include "crosecrypt.hpp"

PacketCodec *g_Crypt = nullptr;

TEST( RoseCrypt, Default )
{
	EXPECT_NO_FATAL_FAILURE( g_Crypt = new PacketCodec() );
}

TEST( RoseCrypt, EncryptData )
{
	CPacket pak( ePacketType::PAKCS_CHAR_LIST_REQ, sizeof(pakChannelList_Req) );
	pak.pChannelListReq.lServerID = 0x77;

	EXPECT_NO_FATAL_FAILURE( g_Crypt->encodeServerPacket( pak.Data ) );

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

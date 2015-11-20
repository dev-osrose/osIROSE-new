//Unit test driver
#ifdef _WIN32
#pragma warning (push, 1)
#pragma warning(disable: 6326 28182 4616)
#endif

#include "gtest/gtest.h"

#ifdef _WIN32
#pragma warning (pop)
#endif

#include <stdint.h>
#include "ePacketType.h"
#include "crosecrypt.hpp"

CPC g_Crypt;

TEST( RoseCrypt, Default )
{
	EXPECT_NO_FATAL_FAILURE( g_Crypt.Default( ) );
}

TEST( RoseCrypt, EncryptData )
{
	g_Crypt.Default();

	CPacket pak( ePacketType::PAKCS_CHAR_LIST_REQ, sizeof(pakChannelList_Req) );
	pak.pChannelListReq.lServerID = 0x77;

	EXPECT_NO_FATAL_FAILURE( g_Crypt.ESSP( pak.Data ) );

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
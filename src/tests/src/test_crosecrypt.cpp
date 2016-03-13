// Unit test driver
#include "gtest/gtest.h"

#include <stdint.h>
#include "epackettype.h"
#include "crosepacket.h"
#include "crosecrypt.h"

using namespace RoseCommon;

// PacketCodec *g_Crypt = nullptr;

TEST(TestRoseCrypt, TestDefaultCrypt) {
  EXPECT_NO_FATAL_FAILURE(PacketCodec g_Crypt);
}

TEST(TestRoseCrypt, TestSetSeed) {
  PacketCodec g_Crypt;
  EXPECT_NO_FATAL_FAILURE(g_Crypt.changeSeed(0x289012));
}

TEST(TestRoseCrypt, TestEncryptData) {
  PacketCodec g_Crypt;

  CRosePacket pak(ePacketType::PAKCS_CHAR_LIST_REQ, sizeof(pakChannelList_Req));
  pak.pChannelListReq.lServerID = 0x77;

  EXPECT_NO_FATAL_FAILURE(g_Crypt.encodeServerPacket(pak.Buffer));

  EXPECT_NE(sizeof(pakChannelList_Req), pak.pChannelListReq.Size);
  EXPECT_NE(ePacketType::PAKCS_CHAR_LIST_REQ, pak.pChannelListReq.Command);
  EXPECT_NE(0x77, pak.pChannelListReq.lServerID);

  EXPECT_EQ(sizeof(pakChannelList_Req),
            g_Crypt.decodeServerHeader((unsigned char*)&pak.Buffer));
  EXPECT_EQ(true, g_Crypt.decodeServerBody((unsigned char*)&pak.Buffer));

  EXPECT_EQ(ePacketType::PAKCS_CHAR_LIST_REQ, pak.pChannelListReq.Command);
  EXPECT_EQ(0x77, pak.pChannelListReq.lServerID);
}

TEST(TestRoseCrypt, TestDecryptData) {
  PacketCodec g_Crypt;
  CRosePacket pak(ePacketType::PAKCS_CHAR_LIST_REQ, sizeof(pakChannelList_Req));
  pak.pChannelListReq.lServerID = 0x77;

  EXPECT_NO_FATAL_FAILURE(g_Crypt.encodeClientPacket(pak.Buffer));

  EXPECT_EQ(sizeof(pakChannelList_Req),
            g_Crypt.decodeClientHeader((unsigned char*)&pak.Buffer));
  EXPECT_EQ(true, g_Crypt.decodeClientBody((unsigned char*)&pak.Buffer));
  EXPECT_EQ(ePacketType::PAKCS_CHAR_LIST_REQ, pak.pChannelListReq.Command);
  EXPECT_EQ(0x77, pak.pChannelListReq.lServerID);
}

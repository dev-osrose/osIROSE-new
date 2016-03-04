#include "gtest/gtest.h"

#include <stdint.h>
#include "ePacketType.h"
#include "crosepacket.h"
#include "ccharserver.h"
#include "ccharisc.h"
#include "ccharclient.h"

TEST(TestCharServer, TestClientPacketPath) {
  CCharServer network;
  CCharClient netConnect;
  EXPECT_EQ(true, network.Init("127.0.0.1", 29110));
  EXPECT_NO_FATAL_FAILURE(network.Listen());

  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  EXPECT_EQ(true, netConnect.Init("127.0.0.1", 29110));
  EXPECT_NO_FATAL_FAILURE(netConnect.Connect());

  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  CRosePacket* pak = new CRosePacket(ePacketType::PAKCS_ACCEPT_REQ);
  netConnect.Send(pak);

  CRosePacket* pak4 = new CRosePacket(ePacketType::PAKCS_LOGIN_REQ);
  netConnect.Send(pak4);

  CRosePacket* pak2 = new CRosePacket(ePacketType::PAKCS_CHANNEL_LIST_REQ);
  netConnect.Send(pak2);

  CRosePacket* pak5 = new CRosePacket(ePacketType::PAKCS_SRV_SELECT_REQ);
  netConnect.Send(pak5);

  CRosePacket* pak3 = new CRosePacket(ePacketType::PAKCS_ALIVE);
  netConnect.Send(pak3);

  std::this_thread::sleep_for(
      std::chrono::milliseconds(500));  // Change this to condition variables
  // EXPECT_NO_FATAL_FAILURE( netConnect.Disconnect( ) );
  EXPECT_NO_FATAL_FAILURE(netConnect.Shutdown());
  // std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
  EXPECT_NO_FATAL_FAILURE(network.Shutdown());
}

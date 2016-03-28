#include "gtest/gtest.h"

#include <stdint.h>
#include "epackettype.h"
#include "crosepacket.h"
#include "ccharserver.h"
#include "ccharisc.h"
#include "ccharclient.h"
#include "cmapisc.h"

using namespace RoseCommon;

TEST(TestCharServer, TestClientPacketPath) {
  CCharServer network;
  CCharClient netConnect;
  EXPECT_EQ(true, network.Init("127.0.0.1", 29112));
  EXPECT_NO_FATAL_FAILURE(network.Listen());

  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  EXPECT_EQ(true, netConnect.Init("127.0.0.1", 29112));
  EXPECT_NO_FATAL_FAILURE(netConnect.Connect());

  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  /* CRosePacket* pak = new CRosePacket(ePacketType::PAKCS_ACCEPT_REQ); */
  /* netConnect.Send(pak); */

  /* CRosePacket* pak4 = new CRosePacket(ePacketType::PAKCS_LOGIN_REQ); */
  /* netConnect.Send(pak4); */

  /* CRosePacket* pak2 = new CRosePacket(ePacketType::PAKCS_CHANNEL_LIST_REQ); */
  /* netConnect.Send(pak2); */

  /* CRosePacket* pak5 = new CRosePacket(ePacketType::PAKCS_SRV_SELECT_REQ); */
  /* netConnect.Send(pak5); */

  /* CRosePacket* pak3 = new CRosePacket(ePacketType::PAKCS_ALIVE); */
  /* netConnect.Send(pak3); */

  std::this_thread::sleep_for(
      std::chrono::milliseconds(500));  // Change this to condition variables
  // EXPECT_NO_FATAL_FAILURE( netConnect.Disconnect( ) );
  EXPECT_NO_FATAL_FAILURE(netConnect.Shutdown());
  // std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
  EXPECT_NO_FATAL_FAILURE(network.Shutdown());
}

TEST(TestCharServer, TestISCMap) {
  CCharServer network;
  CMapISC mapISC;
  EXPECT_EQ(true, network.Init("127.0.0.1", 29112));
  EXPECT_NO_FATAL_FAILURE(network.Listen());

  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  mapISC.SetId(0);
  mapISC.SetType(3);
  EXPECT_EQ(true, mapISC.Init("127.0.0.1", 29112));
  EXPECT_NO_FATAL_FAILURE(mapISC.Connect());

  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  // Create a map connection here

  std::this_thread::sleep_for(
      std::chrono::milliseconds(500));  // Change this to condition variables

  EXPECT_NO_FATAL_FAILURE(mapISC.Shutdown());
  EXPECT_NO_FATAL_FAILURE(network.Shutdown());
}

#include "gtest/gtest.h"

#include <stdint.h>
#include "epackettype.h"
#include "crosepacket.h"
#include "cloginserver.h"
#include "ccharisc.h"
#include "mock/mock_cloginclient.h"
#include "mock/mock_cloginisc.h"

using namespace RoseCommon;

TEST(TestLoginServer, TestClientPacketPath) {
  CLoginServer network;
  CLoginISC* iscServ = new CLoginISC();
  CLoginClient_Mock netConnect;
  EXPECT_EQ(true, network.Init("127.0.0.1", 29110));
  EXPECT_NO_FATAL_FAILURE(network.Listen());

  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  EXPECT_EQ(true, netConnect.Init("127.0.0.1", 29110));
  EXPECT_NO_FATAL_FAILURE(netConnect.Connect());


  std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
  auto pak = std::unique_ptr<CliAcceptReq>(new CliAcceptReq());
  netConnect.Send(*pak);

  //TODO(raven): Move this into a static function so we can just call the function
  //TODO(raven): SendLogin(&netConnect, "test2", "cc03e747a6afbbcbf8be7668acfebee5");
  // cc03e747a6afbbcbf8be7668acfebee5 == test123
  auto pak2 = std::unique_ptr<CliLoginReq>(new CliLoginReq("test2", "cc03e747a6afbbcbf8be7668acfebee5"));
  netConnect.Send(*pak2);

  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  iscServ->SetId(0);
  iscServ->SetType(1);
  CLoginServer::GetISCList().push_front(iscServ);


  //-----------------------------------------
  // We aren't logged in yet
  // We should get a warning
  //-----------------------------------------
  auto pak3 = std::unique_ptr<CRosePacket>(new CliChannelReq(1));
  netConnect.Send(*pak3);

  auto pak4 = std::unique_ptr<CRosePacket>(new CliServerSelectReq(0, 0));
  netConnect.Send(*pak4);
  //-----------------------------------------

  //Incorrect Password
  pak2 = std::unique_ptr<CliLoginReq>(new CliLoginReq("test", "cc03e747a6afbbcbf8be7668acfebee6"));
  netConnect.Send(*pak2);

  //Correct password
  pak2 = std::unique_ptr<CliLoginReq>(new CliLoginReq("test", "cc03e747a6afbbcbf8be7668acfebee5"));
  netConnect.Send(*pak2);

  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  pak3 = std::unique_ptr<CRosePacket>(new CliChannelReq(1));
  netConnect.Send(*pak3);

  pak4 = std::unique_ptr<CRosePacket>(new CliServerSelectReq(0,0));
  netConnect.Send(*pak4);

  auto pak5 = std::unique_ptr<CRosePacket>(new CliAlive());
  netConnect.Send(*pak5);


  std::this_thread::sleep_for(
      std::chrono::milliseconds(100));  // Change this to condition variables

  CLoginServer::GetISCList().clear();

  EXPECT_NO_FATAL_FAILURE(netConnect.Shutdown());
  EXPECT_NO_FATAL_FAILURE(network.Shutdown());
}

TEST(TestLoginServer, TestISCRosePacketPath) {

  CLoginServer network(true);
  CCharISC netConnect;
  EXPECT_EQ(true, network.Init("127.0.0.1", 29111));
  EXPECT_NO_FATAL_FAILURE(network.Listen());

  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  EXPECT_EQ(true, netConnect.Init("127.0.0.1", 29111));
  EXPECT_NO_FATAL_FAILURE(netConnect.Connect());

  std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
//  CRosePacket* pak = new CRosePacket( ePacketType::ISC_ALIVE );
//  netConnect.Send( pak );
//  {
//    uint8_t serverCount = 0;
//    std::lock_guard<std::mutex> lock(CLoginServer::GetISCListMutex());
//    for (auto& server : CLoginServer::GetISCList()) {
//      if (server->GetType() == 1) serverCount++;
//    }
//
//    EXPECT_EQ(1, serverCount);
//  }

  std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
//  CRosePacket* pak2 = new CRosePacket( ePacketType::ISC_ALIVE );
//  netConnect.Send( pak2 );

  std::this_thread::sleep_for(
      std::chrono::milliseconds(500));  // Change this to condition variables
  //EXPECT_NO_FATAL_FAILURE(netConnect.Disconnect());
  //EXPECT_NO_FATAL_FAILURE(netConnect.Shutdown());

  EXPECT_NO_FATAL_FAILURE(network.Shutdown());
}

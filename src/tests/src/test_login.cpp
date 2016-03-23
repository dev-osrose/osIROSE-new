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
  CLoginClient_Mock netConnect;
  EXPECT_EQ(true, network.Init("127.0.0.1", 29110));
  EXPECT_NO_FATAL_FAILURE(network.Listen());

  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  EXPECT_EQ(true, netConnect.Init("127.0.0.1", 29110));
  EXPECT_NO_FATAL_FAILURE(netConnect.Connect());

  std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
  auto pak = std::unique_ptr<CRosePacket>(new CRosePacket(ePacketType::PAKCS_ACCEPT_REQ));
  netConnect.Send(pak);

  auto pak2 = std::unique_ptr<CRosePacket>(new CRosePacket(ePacketType::PAKCS_LOGIN_REQ));
  pak2->AddString("cc03e747a6afbbcbf8be7668acfebee5", false);
  pak2->AddString("test2", true);
  netConnect.Send(pak2);

  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  iscServ->SetId(0);
  iscServ->SetType(1);
  CLoginServer::GetISCList().push_front(iscServ);

  //-----------------------------------------
  // We aren't logged in yet
  // We should get a warning
  //-----------------------------------------
  pak2 = std::unique_ptr<CRosePacket>(new CRosePacket(ePacketType::PAKCS_CHANNEL_LIST_REQ));
  pak2->pChannelListReq.lServerID = 1;
  netConnect.Send(pak2);

  pak2 = std::unique_ptr<CRosePacket>(new CRosePacket(ePacketType::PAKCS_SRV_SELECT_REQ));
  pak2->Add<uint32_t>(0);
  pak2->Add<uint8_t>(0);
  netConnect.Send(pak2);
  //-----------------------------------------

  //Incorrect Password
  auto pak3 = std::unique_ptr<CRosePacket>(new CRosePacket(ePacketType::PAKCS_LOGIN_REQ));
  pak3->AddString("cc03e747a6afbbcbf8be7668acfebee6", false);
  pak3->AddString("test", true);
  netConnect.Send(pak3);

  //Correct password
  pak3 = std::unique_ptr<CRosePacket>(new CRosePacket(ePacketType::PAKCS_LOGIN_REQ));
  pak3->AddString("cc03e747a6afbbcbf8be7668acfebee5", false);
  pak3->AddString("test", true);
  netConnect.Send(pak3);

  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  auto pak4 = std::unique_ptr<CRosePacket>(new CRosePacket(ePacketType::PAKCS_CHANNEL_LIST_REQ));
  pak4->pChannelListReq.lServerID = 1;
  netConnect.Send(pak4);

  auto pak5 = std::unique_ptr<CRosePacket>(new CRosePacket(ePacketType::PAKCS_SRV_SELECT_REQ));
  pak5->Add<uint32_t>(0);
  pak5->Add<uint8_t>(0);
  netConnect.Send(pak5);

  auto pak6 = std::unique_ptr<CRosePacket>(new CRosePacket(ePacketType::PAKCS_ALIVE);
  netConnect.Send(pak6);

  std::this_thread::sleep_for(
      std::chrono::milliseconds(100));  // Change this to condition variables

//  CLoginServer::GetISCList().clear();

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
  CRosePacket* pak = new CRosePacket( ePacketType::ISC_ALIVE );
  netConnect.Send( pak );
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
  CRosePacket* pak2 = new CRosePacket( ePacketType::ISC_ALIVE );
  netConnect.Send( pak2 );

  std::this_thread::sleep_for(
      std::chrono::milliseconds(500));  // Change this to condition variables
  EXPECT_NO_FATAL_FAILURE(netConnect.Disconnect());
  EXPECT_NO_FATAL_FAILURE(netConnect.Shutdown());

  EXPECT_NO_FATAL_FAILURE(network.Shutdown());
}

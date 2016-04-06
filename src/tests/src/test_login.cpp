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

//  iscServ->SetId(0);
//  iscServ->SetType(1);
//  CLoginServer::GetISCList().push_front(iscServ);

  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  EXPECT_EQ(true, netConnect.Init("127.0.0.1", 29110));
  EXPECT_NO_FATAL_FAILURE(netConnect.Connect());

  std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
  CRosePacket* pak = new CRosePacket(ePacketType::PAKCS_ACCEPT_REQ);
  netConnect.Send(pak);

  CRosePacket* pak2 = new CRosePacket(ePacketType::PAKCS_LOGIN_REQ);
  pak2->AddString("cc03e747a6afbbcbf8be7668acfebee5", false);
  pak2->AddString("test", true);
  netConnect.Send(pak2);

  iscServ->SetId(0);
  iscServ->SetType(1);
  CLoginServer::GetISCList().push_front(iscServ);

  //-----------------------------------------
  // We aren't logged in yet
  // We should get a warning
  //-----------------------------------------
  pak2 = new CRosePacket(ePacketType::PAKCS_CHANNEL_LIST_REQ);
  pak2->pChannelListReq.lServerID = 1;
  netConnect.Send(pak2);

  pak2 = new CRosePacket(ePacketType::PAKCS_SRV_SELECT_REQ);
  pak2->Add<uint32_t>(0);
  pak2->Add<uint8_t>(0);
  netConnect.Send(pak2);

  //-----------------------------------------

  CRosePacket* pak3 = new CRosePacket(ePacketType::PAKCS_LOGIN_REQ);
  pak3->AddString("cc03e747a6afbbcbf8be7668acfebee5", false);
  pak3->AddString("test", true);
  netConnect.Send(pak3);

  CRosePacket* pak4 = new CRosePacket(ePacketType::PAKCS_CHANNEL_LIST_REQ);
  pak4->pChannelListReq.lServerID = 1;
  netConnect.Send(pak4);

  CRosePacket* pak5 = new CRosePacket(ePacketType::PAKCS_SRV_SELECT_REQ);
  pak5->Add<uint32_t>(0);
  pak5->Add<uint8_t>(0);
  netConnect.Send(pak5);

  CRosePacket* pak6 = new CRosePacket(ePacketType::PAKCS_ALIVE);
  netConnect.Send(pak6);

  std::this_thread::sleep_for(
      std::chrono::milliseconds(500));  // Change this to condition variables

  EXPECT_NO_FATAL_FAILURE(netConnect.Shutdown());
  EXPECT_NO_FATAL_FAILURE(network.Shutdown());
}

TEST(TestLoginServer, TestISCRosePacketPath) {

  CLoginServer network(true);
  CCharISC netConnect;
  EXPECT_EQ(true, network.Init("127.0.0.1", 29110));
  EXPECT_NO_FATAL_FAILURE(network.Listen());

  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  EXPECT_EQ(true, netConnect.Init("127.0.0.1", 29110));
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

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

  iscServ->SetId(0);
  iscServ->SetType(1);
  CLoginServer::GetISCList().push_front(new CLoginISC());

  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  EXPECT_EQ(true, netConnect.Init("127.0.0.1", 29110));
  EXPECT_NO_FATAL_FAILURE(netConnect.Connect());

  // std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
  auto pak = std::unique_ptr<CRosePacket>(new CRosePacket(ePacketType::PAKCS_ACCEPT_REQ));
  netConnect.Send(pak);

  auto pak4 = std::unique_ptr<CRosePacket>(new CRosePacket(ePacketType::PAKCS_LOGIN_REQ));
  pak4->AddString("cc03e747a6afbbcbf8be7668acfebee5", false);
  pak4->AddString("test", false);
  netConnect.Send(pak4);

  auto pak2 = std::uniaue_ptr<CRosePacket>(new CRosePacket(ePacketType::PAKCS_CHANNEL_LIST_REQ));
  pak2->pChannelListReq.lServerID = 1;
  netConnect.Send(pak2);

  auto pak5 = std::unique_ptr<CRosePacket>(new CRosePacket(ePacketType::PAKCS_SRV_SELECT_REQ));
  pak5->Add<uint32_t>(0);
  pak5->Add<uint8_t>(0);
  netConnect.Send(pak5);

  auto pak3 = std::unique_ptr<CRosePacket>(new CRosePacket(ePacketType::PAKCS_ALIVE));
  netConnect.Send(pak3);

  std::this_thread::sleep_for(
      std::chrono::milliseconds(500));  // Change this to condition variables
  // EXPECT_NO_FATAL_FAILURE( netConnect.Disconnect( ) );
  EXPECT_NO_FATAL_FAILURE(netConnect.Shutdown());
  // std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
  EXPECT_NO_FATAL_FAILURE(network.Shutdown());
}

TEST(TestLoginServer, TestISCRosePacketPath) {
  //	CLogConsole::SetDisplayOmittable( true );

/*  CLoginServer network(true);
  CCharISC netConnect;
  EXPECT_EQ(true, network.Init("127.0.0.1", 29110));
  EXPECT_NO_FATAL_FAILURE(network.Listen());

  //std::this_thread::sleep_for(std::chrono::milliseconds(500));
  EXPECT_EQ(true, netConnect.Init("127.0.0.1", 29110));
  EXPECT_NO_FATAL_FAILURE(netConnect.Connect());

  //std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
  CRosePacket* pak = new CRosePacket( ePacketType::ISC_ALIVE );
  netConnect.Send( pak );
  {
    uint8_t serverCount = 0;
    std::lock_guard<std::mutex> lock(CLoginServer::GetISCListMutex());
    for (auto& server : CLoginServer::GetISCList()) {
      if (server->GetType() == 1) serverCount++;
    }

    EXPECT_EQ(1, serverCount);
  }

  // std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
  //        CRosePacket* pak = new CRosePacket( ePacketType::ISC_ALIVE );
  //        netConnect.Send( pak );

  std::this_thread::sleep_for(
      std::chrono::milliseconds(500));  // Change this to condition variables
  EXPECT_NO_FATAL_FAILURE(netConnect.Disconnect());
  EXPECT_NO_FATAL_FAILURE(netConnect.Shutdown());

  EXPECT_NO_FATAL_FAILURE(network.Shutdown());
*/
  //	CLogConsole::SetDisplayOmittable( false );
}

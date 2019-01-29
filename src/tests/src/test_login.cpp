#include "gtest/gtest.h"

#include <stdint.h>
#include "epackettype.h"
#include "crosepacket.h"
#include "cloginserver.h"
#include "ccharisc.h"
#include "mock/login/mock_cloginclient.h"
#include "mock/login/mock_cloginisc.h"
#include "cnetwork_asio.h"
#include "cli_accept_req.h"
#include "cli_login_req.h"
#include "cli_channel_list_req.h"
#include "cli_srv_select_req.h"
#include "cli_alive.h"
#include "connection.h"
#include "config.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;

TEST(TestLoginServer, TestClientPacketPath) {
  Core::Config& config = Core::Config::getInstance();
  Core::connectionPool.addConnector<Core::Osirose>(std::bind(
                                    Core::mysqlFactory,
                                    config.database().user,
                                    config.database().password,
                                    config.database().database,
                                    config.database().host,
                                    config.database().port));
CLoginServer network;
std::unique_ptr<CLoginISC> iscServ = std::make_unique<CLoginISC>();
CLoginClient_Mock netConnect;

  iscServ->set_socket(std::make_unique<Core::CNetwork_Asio>());
  netConnect.set_socket(std::make_unique<Core::CNetwork_Asio>());

  EXPECT_EQ(true, network.init("127.0.0.1", 29110));
  EXPECT_EQ(true, network.listen());

  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  EXPECT_EQ(true, netConnect.init("127.0.0.1", 29110));
  EXPECT_EQ(true, netConnect.connect());


  std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
  netConnect.send(CliAcceptReq::create());

  //TODO(raven): Move this into a static function so we can just call the function
  //TODO(raven): SendLogin(&netConnect, "test2", "cc03e747a6afbbcbf8be7668acfebee5");
  // cc03e747a6afbbcbf8be7668acfebee5 == test123
  netConnect.send(CliLoginReq::create(std::string("cc03e747a6afbbcbf8be7668acfebee5"), "test"));

  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  iscServ->set_id(0);
  iscServ->set_type(1);
  network.GetISCList().push_front(std::move(iscServ));


  //-----------------------------------------
  // We aren't logged in yet
  // We should get a warning
  //-----------------------------------------
  netConnect.send(CliChannelListReq::create(1));

  netConnect.send(CliSrvSelectReq::create(0, 0));
  //-----------------------------------------

  //Incorrect Password
  netConnect.send(CliLoginReq::create(std::string("cc03e747a6afbbcbf8be7668acfebee6"), "test"));

  //Correct password
  netConnect.send(CliLoginReq::create(std::string("cc03e747a6afbbcbf8be7668acfebee5"), "test"));

  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  netConnect.send(CliChannelListReq::create(1));

  netConnect.send(CliSrvSelectReq::create(0, 0));

  netConnect.send(CliAlive::create());


  std::this_thread::sleep_for(
      std::chrono::milliseconds(1000));  // Change this to condition variables

  EXPECT_NO_FATAL_FAILURE(netConnect.disconnect());

  network.GetISCList().clear();

  EXPECT_NO_FATAL_FAILURE(netConnect.shutdown());
  EXPECT_NO_FATAL_FAILURE(network.shutdown());
}

TEST(TestLoginServer, TestISCRosePacketPath) {

  CLoginServer network(true);
  CCharISC netConnect;
  netConnect.set_socket(std::make_unique<Core::CNetwork_Asio>());

  EXPECT_EQ(true, network.init("127.0.0.1", 29111));
  EXPECT_NO_FATAL_FAILURE(network.listen());

  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  EXPECT_EQ(true, netConnect.init("127.0.0.1", 29111));
  EXPECT_NO_FATAL_FAILURE(netConnect.connect());

  std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
//  CRosePacket* pak = new CRosePacket( ePacketType::ISC_ALIVE );
//  netConnect.send_data( pak );
//  {
//    uint8_t serverCount = 0;
//    std::lock_guard<std::mutex> lock(CLoginServer::GetISCListMutex());
//    for (auto& server : CLoginServer::GetISCList()) {
//      if (server->get_type() == 1) serverCount++;
//    }
//
//    EXPECT_EQ(1, serverCount);
//  }

  std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
//  CRosePacket* pak2 = new CRosePacket( ePacketType::ISC_ALIVE );
//  netConnect.send_data( pak2 );

  std::this_thread::sleep_for(
      std::chrono::milliseconds(500));  // Change this to condition variables
  //EXPECT_NO_FATAL_FAILURE(netConnect.disconnect());
  //EXPECT_NO_FATAL_FAILURE(netConnect.shutdown());

  EXPECT_NO_FATAL_FAILURE(network.shutdown(true));
}

#include "gtest/gtest.h"

#include <stdint.h>
#include "epackettype.h"
#include "crosepacket.h"
#include "ccharserver.h"
#include "ccharisc.h"
#include "ccharclient.h"
#include "cmapisc.h"

#include "cmysql_database.h"
#include "cmysql_databasepool.h"
#include "database.h"
#include "mock/login/mock_cloginclient.h"
#include "cnetwork_asio.h"

using namespace RoseCommon;

TEST(TestCharServer, TestClientPacketPath) {
  CCharServer network;
  CLoginClient_Mock netConnect;
  netConnect.set_socket(new Core::CNetwork_Asio());
  EXPECT_EQ(true, network.init("127.0.0.1", 29112));
  EXPECT_NO_FATAL_FAILURE(network.listen());

  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  EXPECT_EQ(true, netConnect.init("127.0.0.1", 29112));
  EXPECT_NO_FATAL_FAILURE(netConnect.connect());
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  {
    std::string query = fmt::format("CALL create_session({}, {}, {});", 1, 1, 0);

    Core::IDatabase& database = Core::databasePool.getDatabase();
    database.QExecute(query);
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  {
    auto pak = std::unique_ptr<CliJoinServerReq>(
        new CliJoinServerReq(1, "cc03e747a6afbbcbf8be7668acfebee5"));
    netConnect.send(*pak);
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  {
    auto pak = std::unique_ptr<CliCreateCharReq>(
        new CliCreateCharReq(1, 1, 1, 1, 1, 10, "Raven"));
    netConnect.send(*pak);
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  {
    auto pak =
        std::unique_ptr<CliDeleteCharReq>(new CliDeleteCharReq(1, 0, "Raven"));
    netConnect.send(*pak);
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  {
    auto pak = std::unique_ptr<CliCreateCharReq>(
        new CliCreateCharReq(1, 1, 1, 1, 1, 10, "Raven"));
    netConnect.send(*pak);
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  {
    auto pak = std::unique_ptr<CliSelectCharReq>(
        new CliSelectCharReq(1, 0, 0, "Raven"));
    netConnect.send(*pak);
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  std::string query = fmt::format("DELETE FROM sessions WHERE id = {}", 1);

  Core::IDatabase& database = Core::databasePool.getDatabase();
  database.QExecute(query);

  std::this_thread::sleep_for(
      std::chrono::milliseconds(500));  // Change this to condition variables
  // EXPECT_NO_FATAL_FAILURE( netConnect.disconnect( ) );
  EXPECT_NO_FATAL_FAILURE(netConnect.shutdown());
  // std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
  EXPECT_NO_FATAL_FAILURE(network.shutdown());
}

TEST(TestCharServer, TestISCMap) {
  CCharServer network;
  CMapISC mapISC;
  mapISC.set_socket(new Core::CNetwork_Asio());
  EXPECT_EQ(true, network.init("127.0.0.1", 29112));
  EXPECT_NO_FATAL_FAILURE(network.listen());

  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  mapISC.set_id(0);
  mapISC.set_type(3);
  EXPECT_EQ(true, mapISC.init("127.0.0.1", 29112));
  EXPECT_NO_FATAL_FAILURE(mapISC.connect());

  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  // todo(raven): Create a map connection here by crafting a isc packet

  std::this_thread::sleep_for(
      std::chrono::milliseconds(500));  // Change this to condition variables

  EXPECT_NO_FATAL_FAILURE(mapISC.shutdown());
  EXPECT_NO_FATAL_FAILURE(network.shutdown());
}

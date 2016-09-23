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

using namespace RoseCommon;

TEST(TestCharServer, TestClientPacketPath) {
  CCharServer network;
  CLoginClient_Mock netConnect;
  EXPECT_EQ(true, network.Init("127.0.0.1", 29112));
  EXPECT_NO_FATAL_FAILURE(network.Listen());

  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  EXPECT_EQ(true, netConnect.Init("127.0.0.1", 29112));
  EXPECT_NO_FATAL_FAILURE(netConnect.Connect());
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
    netConnect.Send(*pak);
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  {
    auto pak = std::unique_ptr<CliCreateCharReq>(
        new CliCreateCharReq("Raven", 1, 1, 1, 1, 1, 10));
    netConnect.Send(*pak);
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  {
    auto pak =
        std::unique_ptr<CliDeleteCharReq>(new CliDeleteCharReq("Raven", 1, 0));
    netConnect.Send(*pak);
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  {
    auto pak = std::unique_ptr<CliSelectCharReq>(
        new CliSelectCharReq("Raven", 1, 0, 0));
    netConnect.Send(*pak);
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  std::string query = fmt::format("DELETE FROM sessions WHERE id = {}", 1);

  Core::IDatabase& database = Core::databasePool.getDatabase();
  database.QExecute(query);

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

  // todo(raven): Create a map connection here by crafting a isc packet

  std::this_thread::sleep_for(
      std::chrono::milliseconds(500));  // Change this to condition variables

  EXPECT_NO_FATAL_FAILURE(mapISC.Shutdown());
  EXPECT_NO_FATAL_FAILURE(network.Shutdown());
}

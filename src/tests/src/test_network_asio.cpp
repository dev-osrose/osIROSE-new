#include "gtest/gtest.h"

#include <mutex>
#include <condition_variable>
#include <stdint.h>
#include "mock/mock_network_asio.h"
#include "ePacketType.h"
#include "logconsole.h"
//#include "croseserver.h"

// using ::testing::Expectation;

TEST(TestAsioNetworking, TestInit) {
  Core::CNetwork_Asio network;
  // network.SetExtraMessageInfo(true);
  EXPECT_EQ(true, network.Init("127.0.0.1", 29000));
  EXPECT_EQ(0, network.GetId());
  EXPECT_EQ(0, network.GetType());
  EXPECT_EQ(29000, network.GetPort());
  EXPECT_EQ("127.0.0.1", network.GetIpAddress());
  // network.SetExtraMessageInfo(false);
}

TEST(TestAsioNetworking, TestInitHostLessThanTwo) {
  Core::CNetwork_Asio network;
  EXPECT_EQ(false, network.Init("0", 29000));
  EXPECT_NE(29000, network.GetPort());
  EXPECT_NE("0", network.GetIpAddress());
}

TEST(TestAsioNetworking, TestConnectIp) {
  Core::CNetwork_Asio network;
  EXPECT_EQ(true,
            network.Init("63.117.14.24",
                         80));  // We are going to connect to google's website
  EXPECT_NO_FATAL_FAILURE(network.Connect());
  EXPECT_NO_FATAL_FAILURE(network.Shutdown());
}

TEST(TestAsioNetworking, TestRecv) {
  Core::CNetwork_Asio network;
  EXPECT_EQ(true, network.Init("63.117.14.24", 80));
  EXPECT_NO_FATAL_FAILURE(network.Connect());
  EXPECT_NO_FATAL_FAILURE(network.Shutdown());
}

TEST(TestAsioNetworking, TestReconnect) {
  Core::CNetwork_Asio network;
  EXPECT_EQ(true,
            network.Init("63.117.14.24",
                         80));  // We are going to connect to google's website
  EXPECT_NO_FATAL_FAILURE(network.Connect());
  EXPECT_NO_FATAL_FAILURE(network.Disconnect());
  EXPECT_NO_FATAL_FAILURE(network.Reconnect());
  EXPECT_NO_FATAL_FAILURE(network.Shutdown());
}

TEST(TestAsioNetworking, TestConnectHostName) {
  //	CLogConsole log("TestAsioNetworking");
  //	log.icprintf("Construct\n");
  Core::CNetwork_Asio network;
  //	log.icprintf("Init\n");
  EXPECT_NO_FATAL_FAILURE(network.Init(
      "google.com",
      80));  // We are going to connect to google's website using hostname.
  EXPECT_NO_FATAL_FAILURE(network.Connect());
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  EXPECT_NO_FATAL_FAILURE(network.Shutdown());
}

TEST(TestAsioNetworking, TestListen) {
  Core::CNetwork_Asio network;
  EXPECT_EQ(true, network.Init(
                      "127.0.0.1",
                      23124));  // We are going to connect to google's website
  EXPECT_NO_FATAL_FAILURE(network.Listen());
  EXPECT_NO_FATAL_FAILURE(network.Shutdown());
}

TEST(TestAsioNetworking, TestListenAndConnect) {
  Core::CLogConsole log("TestListenAndConnect");

  log.icprintf("Start\n");
  Core::CNetwork_Asio netConnect, network;
  
  log.icprintf( "Init #1\n" );
  EXPECT_EQ( true, network.Init(
                      "127.0.0.1",
                      23456));  // We are going to connect to google's website
  
  log.icprintf( "Listen\n" );
  EXPECT_NO_FATAL_FAILURE( network.Listen() );

  log.icprintf( "Sleep #1\n" );
  std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
  
  log.icprintf( "Init #2\n" );
  EXPECT_EQ( true, netConnect.Init( "127.0.0.1", 23456 ) );
  
  log.icprintf( "Connect\n" );
  EXPECT_NO_FATAL_FAILURE( netConnect.Connect() );

  log.icprintf( "pak\n" );
  CPacket* pak =
      new CPacket(ePacketType::PAKCS_CHAR_LIST_REQ, sizeof(pakChannelList_Req));
  pak->pChannelListReq.lServerID = 0x77;
  
  log.icprintf( "send\n" );
  netConnect.Send( pak->Buffer );

  log.icprintf( "sleep #2\n" );
  std::this_thread::sleep_for(
      std::chrono::milliseconds(100));  // Change this to condition variables
  
  log.icprintf( "Disconnect #2\n" );
  EXPECT_NO_FATAL_FAILURE( netConnect.Disconnect() );
  
  log.icprintf( "Shutdown #2\n" );
  EXPECT_NO_FATAL_FAILURE( netConnect.Shutdown() );

  log.icprintf( "Shutdown #1\n" );
  EXPECT_NO_FATAL_FAILURE(network.Shutdown());
}

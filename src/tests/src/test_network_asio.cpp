#include "gtest/gtest.h"

#include <mutex>
#include <condition_variable>
#include <stdint.h>
#include "mock/core/mock_network_asio.h"
#include "epackettype.h"
#include "crosepacket.h"
#include "logconsole.h"
//#include "croseserver.h"

// using ::testing::Expectation;
using namespace RoseCommon;

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
  //  Core::CNetwork_Asio network;
  //	log.icprintf("Init\n");
  //  EXPECT_NO_FATAL_FAILURE(network.Init(
  //      "google.com",
  //      80));  // We are going to connect to google's website using hostname.
  //  EXPECT_NO_FATAL_FAILURE(network.Connect());
  //  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  //  EXPECT_NO_FATAL_FAILURE(network.Shutdown());
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

  Core::CNetwork_Asio netConnect, network;

  EXPECT_EQ(true, network.Init(
                      "127.0.0.1",
                      23456));  // We are going to connect to google's website

  EXPECT_NO_FATAL_FAILURE(network.Listen());

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  EXPECT_EQ(true, netConnect.Init("127.0.0.1", 23456));
  EXPECT_NO_FATAL_FAILURE(netConnect.Connect());

  struct Test : public CRosePacket {
	  Test(ePacketType type) : CRosePacket(type) {}
	  virtual ~Test() {}
	  protected:
		  void pack() {
			  *this << 0x77;
		  }
  };

  CRosePacket* pak = new Test(ePacketType::PAKCS_CHAR_LIST_REQ);
  netConnect.Send(pak->getPacked());
  EXPECT_NO_FATAL_FAILURE(netConnect.Disconnect());
  EXPECT_NO_FATAL_FAILURE(netConnect.Shutdown());
  EXPECT_NO_FATAL_FAILURE(network.Shutdown());
}

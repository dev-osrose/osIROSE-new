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
  EXPECT_EQ(true, network.init("127.0.0.1", 29000));
  EXPECT_EQ(0, network.get_id());
  EXPECT_EQ(0, network.get_type());
  EXPECT_EQ(29000, network.get_port());
  EXPECT_EQ("127.0.0.1", network.get_address());
  // network.SetExtraMessageInfo(false);
}

TEST(TestAsioNetworking, TestInitHostLessThanTwo) {
  Core::CNetwork_Asio network;
  EXPECT_EQ(false, network.init("0", 29000));
  EXPECT_NE(29000, network.get_port());
  EXPECT_NE("0", network.get_address());
}

TEST(TestAsioNetworking, TestConnectIp) {
  Core::CNetwork_Asio network;
  EXPECT_EQ(true,
            network.init("63.117.14.24",
                         80));  // We are going to connect to google's website
  EXPECT_NO_FATAL_FAILURE(network.connect());
  EXPECT_NO_FATAL_FAILURE(network.shutdown());
}

TEST(TestAsioNetworking, TestRecv) {
  Core::CNetwork_Asio network;
  EXPECT_EQ(true, network.init("63.117.14.24", 80));
  EXPECT_NO_FATAL_FAILURE(network.connect());
  EXPECT_NO_FATAL_FAILURE(network.shutdown());
}

TEST(TestAsioNetworking, TestReconnect) {
  Core::CNetwork_Asio network;
  EXPECT_EQ(true,
            network.init("63.117.14.24",
                         80));  // We are going to connect to google's website
  EXPECT_NO_FATAL_FAILURE(network.connect());
  EXPECT_NO_FATAL_FAILURE(network.disconnect());
  EXPECT_NO_FATAL_FAILURE(network.reconnect());
  EXPECT_NO_FATAL_FAILURE(network.shutdown());
}

TEST(TestAsioNetworking, TestConnectHostName) {
  //	CLogConsole log("TestAsioNetworking");
  //	log.icprintf("Construct\n");
  //  Core::CNetwork_Asio network;
  //	log.icprintf("Init\n");
  //  EXPECT_NO_FATAL_FAILURE(network.Init(
  //      "google.com",
  //      80));  // We are going to connect to google's website using hostname.
  //  EXPECT_NO_FATAL_FAILURE(network.connect());
  //  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  //  EXPECT_NO_FATAL_FAILURE(network.shutdown());
}

TEST(TestAsioNetworking, TestListen) {
  Core::CNetwork_Asio network;
  EXPECT_EQ(true, network.init(
                      "127.0.0.1",
                      23124));  // We are going to connect to google's website
  EXPECT_NO_FATAL_FAILURE(network.listen());
  EXPECT_NO_FATAL_FAILURE(network.shutdown());
}

TEST(TestAsioNetworking, TestListenAndConnect) {

  Core::CNetwork_Asio netConnect, network;

  EXPECT_EQ(true, network.init(
                      "127.0.0.1",
                      23456));  // We are going to connect to google's website

  EXPECT_NO_FATAL_FAILURE(network.listen());

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  EXPECT_EQ(true, netConnect.init("127.0.0.1", 23456));
  EXPECT_NO_FATAL_FAILURE(netConnect.connect());

  struct Test : public CRosePacket {
	  Test(ePacketType type) : CRosePacket(type) {}
	  virtual ~Test() {}
	  protected:
		  void pack() {
			  *this << 0x77;
		  }
  };

  CRosePacket* pak = new Test(ePacketType::PAKCS_CHAR_LIST_REQ);
  netConnect.send_data(pak->getPacked());
  EXPECT_NO_FATAL_FAILURE(netConnect.disconnect());
  EXPECT_NO_FATAL_FAILURE(netConnect.shutdown());
  EXPECT_NO_FATAL_FAILURE(network.shutdown());
}

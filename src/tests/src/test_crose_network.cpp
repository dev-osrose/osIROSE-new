#include "gmock/gmock.h"
#include "gtest/gtest.h"

//#define STRESS_TEST

#include <stdint.h>
#include "croseserver.h"
//#include "mock/rosecommon/mock_croseclient.h"
//#include "mock/rosecommon/mock_croseisc.h"
#include "epackettype.h"
#include "crosepacket.h"
#include "logconsole.h"

using namespace RoseCommon;

TEST(TestRoseNetwork, Constructor) { CRoseServer network; }

TEST(TestRoseNetwork, TestInit) {
  CRoseServer network;
  EXPECT_EQ(true, network.Init("127.0.0.1", 29000));
  EXPECT_EQ(29000, network.GetPort());
  EXPECT_EQ("127.0.0.1", network.GetIpAddress());
}

TEST(TestRoseNetwork, TestInitHostLessThanTwo) {
  CRoseServer network;
  EXPECT_EQ(false, network.Init("0", 29000));
  EXPECT_NE(29000, network.GetPort());
  EXPECT_NE("0", network.GetIpAddress());
}

TEST(TestRoseNetwork, TestConnectIp) {
  //	std::mutex mutex;
  //	std::condition_variable cv;
  //	bool done = false;

  CRoseServer network;
  EXPECT_EQ(true,
            network.Init("63.117.14.24",
                         80));  // We are going to connect to google's website
  EXPECT_NO_FATAL_FAILURE(network.Connect());
  //	EXPECT_CALL( network, OnConnect() )
  //                                .WillOnce(testing::Invoke([&]()->int {
  //                                            std::lock_guard<std::mutex>
  //                                            lock(mutex);
  //                                            done = true;
  //                                            cv.notify_all();
  //                                            return 1; }));

  //	EXPECT_CALL( network, OnConnected() )
  //				.WillOnce(testing::Invoke([&]()->int {
  //				            std::lock_guard<std::mutex>
  //lock(mutex);
  //				            done = true;
  //				            cv.notify_all();
  //				            return 1; }));

  //	std::unique_lock<std::mutex> lock(mutex);
  //	std::unique_lock<std::mutex> lock2(mutex2);
  //	EXPECT_TRUE(cv.wait_for(lock,
  //					std::chrono::seconds(1),
  //					[&done] { return done; })
  //					);
  //	EXPECT_TRUE(cv.wait_for(lock,
  //                                        std::chrono::seconds(1),
  //                                        [&done] { return done; })
  //                                        );
  //	std::this_thread::sleep_for(std::chrono::milliseconds(100));
  EXPECT_NO_FATAL_FAILURE(network.Shutdown());
}

TEST(TestRoseNetwork, TestRecv) {
  CRoseServer network;
  EXPECT_EQ(true, network.Init("63.117.14.24", 80));
  EXPECT_NO_FATAL_FAILURE(network.Connect());
  std::this_thread::sleep_for(std::chrono::milliseconds(
      500));  // Make sure we wait a little for data to come in
  EXPECT_NO_FATAL_FAILURE(network.Shutdown());
}

TEST(TestRoseNetwork, TestReconnect) {
  //	CRoseClient network;
  //	EXPECT_EQ( true, network.Init( "63.117.14.24", 80 ) ); // We are going
  //to connect to google's website
  //	EXPECT_NO_FATAL_FAILURE( network.Connect( ) );
  // EXPECT_NO_FATAL_FAILURE( network.Disconnect( ) );
  //	std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
  //	EXPECT_NO_FATAL_FAILURE( network.Reconnect( ) );
  //	EXPECT_NO_FATAL_FAILURE( network.Shutdown( ) );
}

TEST(TestRoseNetwork, TestConnectHostName) {
  //	::testing::FLAGS_gmock_verbose = "info";
  //	std::mutex mutex;
  //        std::condition_variable cv;
  //        bool done = false;

  //	CRoseServer network;
  //	EXPECT_NO_FATAL_FAILURE( network.Init( "google.com", 80 ) ); // We are
  //going to connect to google's website using hostname.
  //	EXPECT_NO_FATAL_FAILURE( network.Connect( ) );
  //	EXPECT_CALL( network, OnConnect() ).Times(1);
  //	EXPECT_CALL( network, OnConnected() )
  //                                .WillOnce(testing::Invoke([&]()->int {
  //                                            std::lock_guard<std::mutex>
  //                                            lock(mutex);
  //                                            done = true;
  //                                            cv.notify_all();
  //                                            return 1; }));
  //	std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
  //	std::unique_lock<std::mutex> lock(mutex);
  //	EXPECT_TRUE(cv.wait_for(lock,
  //                                        std::chrono::seconds(4),
  //                                        [&done] { return done; })
  //                                        );
  //	EXPECT_NO_FATAL_FAILURE( network.Shutdown( ) );
}

TEST(TestRoseNetwork, TestListen) {
  //	::testing::FLAGS_gmock_verbose = "info";
  //	std::mutex mutex;
  //        std::condition_variable cv;
  //        bool done = false;

  CRoseServer network;
  EXPECT_EQ(true, network.Init(
                      "127.0.0.1",
                      29000));  // We are going to connect to google's website
  EXPECT_NO_FATAL_FAILURE(network.Listen());
  //	EXPECT_CALL( network, OnListening() )
  //                                        .WillOnce(
  //                                        testing::Invoke([&]()->int {
  //                                                        std::lock_guard<std::mutex>
  //                                                        lock(mutex);
  //                                                        done = true;
  //                                                        cv.notify_all();
  //                                                        return 1; }));

  //	std::unique_lock<std::mutex> lock(mutex);
  //        EXPECT_TRUE(cv.wait_for(lock, std::chrono::seconds(1), [&done] {
  //        return done; }));

  EXPECT_NO_FATAL_FAILURE(network.Shutdown());
}

TEST(TestRoseNetwork, TestListenAndConnect) {
  //	std::mutex mutex;
  //	std::condition_variable cv;
  //	bool done = false;

  CRoseServer network;
  CRoseClient netConnect;
  EXPECT_EQ(true, network.Init(
                      "127.0.0.1",
                      29100));  // We are going to connect to google's website
  EXPECT_NO_FATAL_FAILURE(network.Listen());
  //	EXPECT_CALL( network, OnListening() )
  //					.WillOnce( testing::Invoke([&]()->int {
  //							std::lock_guard<std::mutex>
  //lock(mutex);
  //							done = true;
  //							cv.notify_all();
  //							return 1; }));

  //	std::unique_lock<std::mutex> lock(mutex);
  //	EXPECT_TRUE(cv.wait_for(lock, std::chrono::seconds(1), [&done] { return
  //done; }));

  // std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
  EXPECT_EQ(true, netConnect.Init("127.0.0.1", 29100));
  EXPECT_NO_FATAL_FAILURE(netConnect.Connect());

  // std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
  /* CRosePacket* pak = new CRosePacket(ePacketType::PAKCS_CHAR_LIST_REQ, */
                                     /* sizeof(pakChannelList_Req)); */
  /* pak->pChannelListReq.lServerID = 0x77; */
  /* netConnect.Send(pak); */

  //	std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) ); //
  //Change this to condition variables
  EXPECT_NO_FATAL_FAILURE(netConnect.Disconnect());
  EXPECT_NO_FATAL_FAILURE(netConnect.Shutdown());

  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  EXPECT_NO_FATAL_FAILURE(network.Shutdown());
}

TEST(TestRoseNetwork, TestListenAndConnect2) {
  CRoseServer network;
  CRoseClient netConnect;
  EXPECT_EQ(true, network.Init("127.0.0.1", 29110));
  EXPECT_NO_FATAL_FAILURE(network.Listen());

  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  EXPECT_EQ(true, netConnect.Init("127.0.0.1", 29110));
  EXPECT_NO_FATAL_FAILURE(netConnect.Connect());

  // std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
  /* CRosePacket* pak = new CRosePacket(ePacketType::PAKCS_ACCEPT_REQ); */
  /* netConnect.Send(pak); */

  /* CRosePacket* pak2 = new CRosePacket(ePacketType::PAKCS_CHANNEL_LIST_REQ, */
                                      /* sizeof(pakChannelList_Req)); */
  /* pak2->pChannelListReq.lServerID = 0x77; */
  /* netConnect.Send(pak2); */

  /* CRosePacket* pak3 = new CRosePacket(ePacketType::PAKCS_ALIVE); */
  /* netConnect.Send(pak3); */

  //        std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) ); //
  //        Change this to condition variables
  EXPECT_NO_FATAL_FAILURE(netConnect.Disconnect());
  EXPECT_NO_FATAL_FAILURE(netConnect.Shutdown());

  EXPECT_NO_FATAL_FAILURE(network.Shutdown());
}

TEST(TestRoseNetwork, TestISCListenAndConnect) {
  //      std::mutex mutex;
  //      std::condition_variable cv;
  //      bool done = false;

  CRoseServer network(true);
  CRoseISC netConnect;
  EXPECT_EQ(true, network.Init(
                      "127.0.0.1",
                      29110));  // We are going to connect to google's website
  EXPECT_NO_FATAL_FAILURE(network.Listen());
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  EXPECT_EQ(true, netConnect.Init("127.0.0.1", 29110));
  EXPECT_NO_FATAL_FAILURE(netConnect.Connect());

  /* CRosePacket* pak = new CRosePacket(ePacketType::ISC_ALIVE); */
  /* netConnect.Send(pak); */

  std::this_thread::sleep_for(
      std::chrono::milliseconds(500));  // Change this to condition variables
  EXPECT_NO_FATAL_FAILURE(netConnect.Disconnect());
  EXPECT_NO_FATAL_FAILURE(netConnect.Shutdown());

  EXPECT_NO_FATAL_FAILURE(network.Shutdown());
}
#ifdef STRESS_TEST

static uint32_t stress_index = 0;
#ifndef WIN32
#include <sys/time.h>
#include <chrono>

unsigned int GetTickCount() {
  //        struct timeval tv;
  //        if(gettimeofday(&tv, NULL) != 0)
  //                return 0;

  //        return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::high_resolution_clock::now().time_since_epoch())
      .count();
}
#endif

TEST(TestRoseNetwork, TestNetworkStress) {
  static Core::CLogConsole log("TestNetworkStress");
  CRoseServer network;
  EXPECT_EQ(true, network.Init("127.0.0.1", 29111));
  EXPECT_NO_FATAL_FAILURE(network.Listen());

  std::thread io_thread_[1000];
  for (int idx = 0; idx < 1000; idx++) {
    io_thread_[idx] = std::thread([this]() {
      auto starttime = GetTickCount();
      CRoseClient_Mock netConnect;
      netConnect.SetId(stress_index++);

      EXPECT_EQ(true, netConnect.Init("127.0.0.1", 29111));
      EXPECT_NO_FATAL_FAILURE(netConnect.Connect());

      /* CRosePacket* pak = new CRosePacket(ePacketType::PAKCS_ALIVE); */
      /* netConnect.Send(pak); */
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      netConnect.Disconnect();
      auto diff = GetTickCount() - starttime;

      log.icprintf("[%d] Completed in %d ms\n", netConnect.GetId(), diff);
      return 0;
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(400));
  log.icprintf("Waiting for threads to finish\n");
  for (int idx = 0; idx < 1000; idx++) {
    io_thread_[idx].join();
  }

  stress_index = 0;
}
#endif

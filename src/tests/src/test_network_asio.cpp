#include "gtest/gtest.h"

#include <mutex>
#include <condition_variable>
#include <stdint.h>
#include "mock/mock_network_asio.h"
//#include "crose_network.h"

//using ::testing::Expectation;

TEST( TestAsioNetworking, TestInit )
{
	CNetwork_Asio network;
	EXPECT_EQ( true, network.Init( "127.0.0.1", 29000 ) );
	EXPECT_EQ( 29000, network.GetPort() );
        EXPECT_EQ( "127.0.0.1", network.GetIpAddress() );
}

TEST( TestAsioNetworking, TestInitHostLessThanTwo )
{
	CNetwork_Asio network;
	EXPECT_EQ( false, network.Init( "0", 29000 ) );
	EXPECT_NE( 29000, network.GetPort() );
	EXPECT_NE( "0", network.GetIpAddress() );
}

TEST( TestAsioNetworking, TestConnectIp )
{
//	std::mutex mutex;
//	std::condition_variable cv;
//	bool done = false;

	CNetwork_Asio network;
	EXPECT_EQ( true, network.Init( "63.117.14.24", 80 ) ); // We are going to connect to google's website
	EXPECT_NO_FATAL_FAILURE( network.Connect( ) );
//	EXPECT_CALL( network, OnConnect() )
//                                .WillOnce(testing::Invoke([&]()->int {
//                                            std::lock_guard<std::mutex> lock(mutex);
//                                            done = true;
//                                            cv.notify_all();
//                                            return 1; }));

//	EXPECT_CALL( network, OnConnected() )
//				.WillOnce(testing::Invoke([&]()->int {
//				            std::lock_guard<std::mutex> lock(mutex);
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
	EXPECT_NO_FATAL_FAILURE( network.Shutdown( ) );
}

TEST( TestAsioNetworking, TestRecv )
{
	CNetwork_Asio network;
	EXPECT_EQ( true, network.Init( "63.117.14.24", 80 ) );
	EXPECT_NO_FATAL_FAILURE( network.Connect( ) );
	std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Make sure we wait a little for data to come in
	EXPECT_NO_FATAL_FAILURE( network.Shutdown( ) );
}

TEST( TestAsioNetworking, TestReconnect )
{
	CNetwork_Asio network;
        EXPECT_EQ( true, network.Init( "63.117.14.24", 80 ) ); // We are going to connect to google's website
        EXPECT_NO_FATAL_FAILURE( network.Connect( ) );
	EXPECT_NO_FATAL_FAILURE( network.Disconnect( ) );
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	EXPECT_NO_FATAL_FAILURE( network.Reconnect( ) );
	EXPECT_NO_FATAL_FAILURE( network.Shutdown( ) );
}

TEST( TestAsioNetworking, TestConnectHostName )
{
//	::testing::FLAGS_gmock_verbose = "info";
//	std::mutex mutex;
//        std::condition_variable cv;
//        bool done = false;

	CNetwork_Asio network;
	EXPECT_NO_FATAL_FAILURE( network.Init( "google.com", 80 ) ); // We are going to connect to google's website using hostname.
	EXPECT_NO_FATAL_FAILURE( network.Connect( ) );
//	EXPECT_CALL( network, OnConnect() ).Times(1);
//	EXPECT_CALL( network, OnConnected() )
//                                .WillOnce(testing::Invoke([&]()->int {
//                                            std::lock_guard<std::mutex> lock(mutex);
//                                            done = true;
//                                            cv.notify_all();
//                                            return 1; }));
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
//	std::unique_lock<std::mutex> lock(mutex);
//	EXPECT_TRUE(cv.wait_for(lock,
//                                        std::chrono::seconds(4),
//                                        [&done] { return done; })
//                                        );
	EXPECT_NO_FATAL_FAILURE( network.Shutdown( ) );
}

TEST( TestAsioNetworking, TestListen )
{
//	::testing::FLAGS_gmock_verbose = "info";
//	std::mutex mutex;
//        std::condition_variable cv;
//        bool done = false;

	CNetwork_Asio network;
	EXPECT_EQ( true, network.Init( "127.0.0.1", 23124 ) ); // We are going to connect to google's website
	EXPECT_NO_FATAL_FAILURE( network.Listen() );
//	EXPECT_CALL( network, OnListening() )
//                                        .WillOnce( testing::Invoke([&]()->int {
//                                                        std::lock_guard<std::mutex> lock(mutex);
//                                                        done = true;
//                                                        cv.notify_all();
//                                                        return 1; }));

//	std::unique_lock<std::mutex> lock(mutex);
//        EXPECT_TRUE(cv.wait_for(lock, std::chrono::seconds(1), [&done] { return done; }));

	EXPECT_NO_FATAL_FAILURE( network.Shutdown() );
}

TEST( TestAsioNetworking, TestListenAndConnect )
{
//	std::mutex mutex;
//	std::condition_variable cv;
//	bool done = false;

	CNetwork_Asio netConnect, network;
	EXPECT_EQ( true, network.Init( "127.0.0.1", 23456 ) ); // We are going to connect to google's website
	EXPECT_NO_FATAL_FAILURE( network.Listen() );
//	EXPECT_CALL( network, OnListening() )
//					.WillOnce( testing::Invoke([&]()->int {
//							std::lock_guard<std::mutex> lock(mutex);
//							done = true;
//							cv.notify_all();
//							return 1; }));

//	std::unique_lock<std::mutex> lock(mutex);
//	EXPECT_TRUE(cv.wait_for(lock, std::chrono::seconds(1), [&done] { return done; }));

	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	EXPECT_EQ( true, netConnect.Init( "127.0.0.1", 23456 ) );
	EXPECT_NO_FATAL_FAILURE( netConnect.Connect( ) );
	std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Change this to condition variables
	EXPECT_NO_FATAL_FAILURE( netConnect.Disconnect( ) );
	EXPECT_NO_FATAL_FAILURE( netConnect.Shutdown( ) );

	EXPECT_NO_FATAL_FAILURE( network.Shutdown() );
}

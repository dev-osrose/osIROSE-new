#include "gtest/gtest.h"

#include <mutex>
#include <condition_variable>
#include <stdint.h>
#include "mock/mock_network_asio.h"

using ::testing::Expectation;

TEST( NetworkingASIO, TestInit )
{
	CNetwork_Asio network;
	EXPECT_EQ( true, network.Init( "127.0.0.1", 29000 ) );
	EXPECT_EQ( 29000, network.GetPort() );
        EXPECT_EQ( "127.0.0.1", network.GetIpAddress() );
}

TEST( NetworkingASIO, TestInitHostLessThanTwo )
{
	CNetwork_Asio network;
	EXPECT_EQ( false, network.Init( "0", 29000 ) );
	EXPECT_NE( 29000, network.GetPort() );
	EXPECT_NE( "0", network.GetIpAddress() );
}

TEST( NetworkingASIO, TestConnectIp )
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
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	EXPECT_NO_FATAL_FAILURE( network.Shutdown( ) );
}

TEST( NetworkingASIO, TestRecv )
{
	CNetwork_Asio network;
	EXPECT_EQ( true, network.Init( "63.117.14.24", 80 ) );
	EXPECT_NO_FATAL_FAILURE( network.Connect( ) );
	std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Make sure we wait a little for data to come in
	EXPECT_NO_FATAL_FAILURE( network.Shutdown( ) );
}

TEST( NetworkingASIO, TestReconnect )
{
	CNetwork_Asio network;
        EXPECT_EQ( true, network.Init( "63.117.14.24", 80 ) ); // We are going to connect to google's website
        EXPECT_NO_FATAL_FAILURE( network.Connect( ) );
	EXPECT_NO_FATAL_FAILURE( network.Disconnect( ) );
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	EXPECT_NO_FATAL_FAILURE( network.Reconnect( ) );
	EXPECT_NO_FATAL_FAILURE( network.Shutdown( ) );
}

TEST( NetworkingASIO, TestConnectHostName )
{
	::testing::FLAGS_gmock_verbose = "info";
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
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
//	std::unique_lock<std::mutex> lock(mutex);
//	EXPECT_TRUE(cv.wait_for(lock,
//                                        std::chrono::seconds(4),
//                                        [&done] { return done; })
//                                        );
	EXPECT_NO_FATAL_FAILURE( network.Shutdown( ) );
}

TEST( NetworkingASIO, TestListen )
{
::testing::FLAGS_gmock_verbose = "info";
//	std::mutex mutex;
//        std::condition_variable cv;
//        bool done = false;

	CNetwork_Asio network;
	EXPECT_EQ( true, network.Init( "127.0.0.1", 29000 ) ); // We are going to connect to google's website
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

TEST( NetworkingASIO, TestListenAndConnect )
{
//	std::mutex mutex;
//	std::condition_variable cv;
//	bool done = false;

	CNetwork_Asio netConnect, network;
	EXPECT_EQ( true, network.Init( "127.0.0.1", 29000 ) ); // We are going to connect to google's website
	EXPECT_NO_FATAL_FAILURE( network.Listen() );
//	EXPECT_CALL( network, OnListening() )
//					.WillOnce( testing::Invoke([&]()->int {
//							std::lock_guard<std::mutex> lock(mutex);
//							done = true;
//							cv.notify_all();
//							return 1; }));

//	std::unique_lock<std::mutex> lock(mutex);
//	EXPECT_TRUE(cv.wait_for(lock, std::chrono::seconds(1), [&done] { return done; }));

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	EXPECT_EQ( true, netConnect.Init( "127.0.0.1", 29000 ) );
	EXPECT_NO_FATAL_FAILURE( netConnect.Connect( ) );
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	EXPECT_NO_FATAL_FAILURE( netConnect.Disconnect( ) );
	EXPECT_NO_FATAL_FAILURE( netConnect.Shutdown( ) );

	EXPECT_NO_FATAL_FAILURE( network.Shutdown() );
}

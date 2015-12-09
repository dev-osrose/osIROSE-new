#include "gtest/gtest.h"

#include <mutex>
#include <condition_variable>
#include <stdint.h>
#include "mock/mock_network_asio.h"
#include "ePacketType.h"
//#include "croseserver.h"

//using ::testing::Expectation;

TEST( TestAsioNetworking, TestInit )
{
	CNetwork_Asio network;
	//network.SetExtraMessageInfo(true);
	EXPECT_EQ( true, network.Init( "127.0.0.1", 29000 ) );
	EXPECT_EQ( 0, network.GetId( ) );
	EXPECT_EQ( 0, network.GetType( ) );
	EXPECT_EQ( 29000, network.GetPort( ) );
	EXPECT_EQ( "127.0.0.1", network.GetIpAddress( ) );
	//network.SetExtraMessageInfo(false);
}

TEST( TestAsioNetworking, TestInitHostLessThanTwo )
{
	CNetwork_Asio network;
	EXPECT_EQ( false, network.Init( "0", 29000 ) );
	EXPECT_NE( 29000, network.GetPort( ) );
	EXPECT_NE( "0", network.GetIpAddress( ) );
}

TEST( TestAsioNetworking, TestConnectIp )
{
	CNetwork_Asio network;
	EXPECT_EQ( true, network.Init( "63.117.14.24", 80 ) ); // We are going to connect to google's website
	EXPECT_NO_FATAL_FAILURE( network.Connect( ) );
	EXPECT_NO_FATAL_FAILURE( network.Shutdown( ) );
}

TEST( TestAsioNetworking, TestRecv )
{
	CNetwork_Asio network;
	EXPECT_EQ( true, network.Init( "63.117.14.24", 80 ) );
	EXPECT_NO_FATAL_FAILURE( network.Connect( ) );
	EXPECT_NO_FATAL_FAILURE( network.Shutdown( ) );
}

TEST( TestAsioNetworking, TestReconnect )
{
	CNetwork_Asio network;
	EXPECT_EQ( true, network.Init( "63.117.14.24", 80 ) ); // We are going to connect to google's website
	EXPECT_NO_FATAL_FAILURE( network.Connect( ) );
	EXPECT_NO_FATAL_FAILURE( network.Disconnect( ) );
	EXPECT_NO_FATAL_FAILURE( network.Reconnect( ) );
	EXPECT_NO_FATAL_FAILURE( network.Shutdown( ) );
}

TEST( TestAsioNetworking, TestConnectHostName )
{
	CNetwork_Asio network;
	EXPECT_NO_FATAL_FAILURE( network.Init( "google.com", 80 ) ); // We are going to connect to google's website using hostname.
	//EXPECT_CALL( network, OnConnect( ) );
	//EXPECT_CALL( network, OnConnected( ) );
	EXPECT_NO_FATAL_FAILURE( network.Connect( ) );
	std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
	//	EXPECT_CALL( network, OnDisconnect() )
	//		.Times( testing::AnyNumber() );
	EXPECT_NO_FATAL_FAILURE( network.Shutdown( ) );
}

TEST( TestAsioNetworking, TestListen )
{
	CNetwork_Asio network;
	EXPECT_EQ( true, network.Init( "127.0.0.1", 23124 ) ); // We are going to connect to google's website
	EXPECT_NO_FATAL_FAILURE( network.Listen( ) );
	EXPECT_NO_FATAL_FAILURE( network.Shutdown( ) );
}

TEST( TestAsioNetworking, TestListenAndConnect )
{
	CNetwork_Asio netConnect, network;
	EXPECT_EQ( true, network.Init( "127.0.0.1", 23456 ) ); // We are going to connect to google's website
	EXPECT_NO_FATAL_FAILURE( network.Listen( ) );

	std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
	EXPECT_EQ( true, netConnect.Init( "127.0.0.1", 23456 ) );
	EXPECT_NO_FATAL_FAILURE( netConnect.Connect( ) );

	CPacket* pak = new CPacket( ePacketType::PAKCS_CHAR_LIST_REQ, sizeof( pakChannelList_Req ) );
	pak->pChannelListReq.lServerID = 0x77;
	netConnect.Send( pak->Buffer );

	std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) ); // Change this to condition variables
	EXPECT_NO_FATAL_FAILURE( netConnect.Disconnect( ) );
	EXPECT_NO_FATAL_FAILURE( netConnect.Shutdown( ) );

	EXPECT_NO_FATAL_FAILURE( network.Shutdown( ) );
}

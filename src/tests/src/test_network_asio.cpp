#include "gtest/gtest.h"

#include <stdint.h>
#include "mock_network_asio.h"


TEST( NetworkingASIO, TestInit )
{
	CNetwork_Asio network;
	EXPECT_EQ( true, network.Init( "127.0.0.1", 29000 ) );
}

TEST( NetworkingASIO, TestInitHostLessThanTwo )
{
	CNetwork_Asio network;
	EXPECT_EQ( false, network.Init( "0", 29000 ) );
}

TEST( NetworkingASIO, TestConnectIp )
{
	CNetwork_Asio_Mock network;
	EXPECT_EQ( true, network.Init( "63.117.14.24", 80 ) ); // We are going to connect to google's website
	EXPECT_NO_FATAL_FAILURE( network.Connect( ) );
	EXPECT_CALL( network, OnConnected() ).Times(1);
	EXPECT_NO_FATAL_FAILURE( network.Shutdown( ) );
}

TEST( NetworkingASIO, TestConnectHostName )
{
	CNetwork_Asio_Mock network;
	EXPECT_NO_FATAL_FAILURE( network.Init( "google.com", 80 ) ); // We are going to connect to google's website using hostname.
	EXPECT_NO_FATAL_FAILURE( network.Connect( ) );
	EXPECT_CALL( network, OnConnected() ).Times(1);
	EXPECT_NO_FATAL_FAILURE( network.Shutdown( ) );
}

TEST( NetworkingASIO, TestListen )
{
	CNetwork_Asio network;
	EXPECT_EQ( true, network.Init( "0.0.0.0", 29000 ) ); // We are going to connect to google's website
	EXPECT_NO_FATAL_FAILURE( network.Listen() );
	EXPECT_NO_FATAL_FAILURE( network.Shutdown() );
}

//TEST( NetworkingASIO, TestListenAndConnect )
//{
//	CNetwork_Asio netConnect, network;
//	EXPECT_EQ( true, network.Init( "0.0.0.0", 29000 ) ); // We are going to connect to google's website
//	EXPECT_NO_FATAL_FAILURE( network.Listen() );
//
//	EXPECT_EQ( true, netConnect.Init( "127.0.0.1", 29000 ) );
//	EXPECT_NO_FATAL_FAILURE( netConnect.Connect( ) );
//	EXPECT_NO_FATAL_FAILURE( netConnect.Disconnect( ) );
//	EXPECT_NO_FATAL_FAILURE( netConnect.Shutdown( ) );
//
//	EXPECT_NO_FATAL_FAILURE( network.Shutdown() );
//}

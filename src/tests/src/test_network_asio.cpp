#include "gtest/gtest.h"

#include <stdint.h>
#include "cnetwork_asio.h"

CNetwork_Asio network;

TEST( NetworkingASIO, TestInit )
{
	EXPECT_EQ(true, network.Init( "127.0.0.1", 29000 ));
}

TEST( NetworkingASIO, TestInitHostLessThanSeven )
{
	EXPECT_EQ( false, network.Init( "0", 29000 ) );
}

TEST( NetworkingASIO, TestConnect )
{
	EXPECT_EQ( true, network.Init( "63.117.14.24", 80 ) ); // We are going to connect to google's website
	EXPECT_NO_FATAL_FAILURE( network.Connect( ) );

	EXPECT_NO_FATAL_FAILURE( network.Disconnect( ) );
	EXPECT_NO_FATAL_FAILURE( network.Shutdown( ) );
}

TEST( NetworkingASIO, TestListen )
{
	EXPECT_EQ( true, network.Init( "0.0.0.0", 29000 ) ); // We are going to connect to google's website
	EXPECT_NO_FATAL_FAILURE( network.Listen() );
	EXPECT_NO_FATAL_FAILURE( network.Shutdown() );
}

//TEST( NetworkingASIO, TestListenAndConnect )
//{
//	CNetwork_Asio netConnect;
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

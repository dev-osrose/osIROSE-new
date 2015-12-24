#include "gtest/gtest.h"

#include <stdint.h>
#include "ePacketType.h"
#include "cloginserver.h"
#include "ccharisc.h"
#include "mock/mock_cloginclient.h"
#include "mock/mock_cloginisc.h"

TEST( TestLoginServer, TestClientPacketPath )
{
	CLoginServer network;
	CLoginISC* iscServ = new CLoginISC();
        CLoginClient_Mock netConnect;
        EXPECT_EQ( true, network.Init( "127.0.0.1", 29110 ) );
        EXPECT_NO_FATAL_FAILURE( network.Listen( ) );

	iscServ->SetId( 0 );
	iscServ->SetType( 1 );
	CLoginServer::GetISCList().push_front(iscServ);

        std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
        EXPECT_EQ( true, netConnect.Init( "127.0.0.1", 29110 ) );
        EXPECT_NO_FATAL_FAILURE( netConnect.Connect( ) );

        //std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
        CPacket* pak = new CPacket( ePacketType::PAKCS_ACCEPT_REQ );
        netConnect.Send( pak );

	CPacket* pak4 = new CPacket( ePacketType::PAKCS_LOGIN_REQ );
        pak4->AddString( "cc03e747a6afbbcbf8be7668acfebee5", false );
	pak4->AddString( "test", false );
        netConnect.Send( pak4 );

        CPacket* pak2 = new CPacket( ePacketType::PAKCS_CHANNEL_LIST_REQ );
	pak2->pChannelListReq.lServerID = 1;
        netConnect.Send( pak2 );

	CPacket* pak5 = new CPacket( ePacketType::PAKCS_SRV_SELECT_REQ );
	pak5->Add<uint32_t>( 0 );
	pak5->Add<uint8_t>( 0 );
        netConnect.Send( pak5 );

        CPacket* pak3 = new CPacket( ePacketType::PAKCS_ALIVE );
        netConnect.Send( pak3 );

        std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) ); // Change this to condition variables
        //EXPECT_NO_FATAL_FAILURE( netConnect.Disconnect( ) );
        EXPECT_NO_FATAL_FAILURE( netConnect.Shutdown( ) );
	//std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
        EXPECT_NO_FATAL_FAILURE( network.Shutdown( ) );
}

TEST( TestLoginServer, TestISCPacketPath )
{
//	CLogConsole::SetDisplayOmittable( true );

        CLoginServer network(true);
        CCharISC netConnect;
        EXPECT_EQ( true, network.Init( "127.0.0.1", 29110 ) );
        EXPECT_NO_FATAL_FAILURE( network.Listen( ) );

        std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
        EXPECT_EQ( true, netConnect.Init( "127.0.0.1", 29110 ) );
        EXPECT_NO_FATAL_FAILURE( netConnect.Connect( ) );

	{
		uint8_t serverCount = 0;
		std::lock_guard< std::mutex > lock( CLoginServer::GetISCListMutex() );
                for( auto& server : CLoginServer::GetISCList() )
		{
			if( server->GetType() == 1)
				serverCount++;
		}

		EXPECT_EQ( 1, serverCount );
	}

        //std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
//        CPacket* pak = new CPacket( ePacketType::ISC_ALIVE );
//        netConnect.Send( pak );

        std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) ); // Change this to condition variables
        EXPECT_NO_FATAL_FAILURE( netConnect.Disconnect( ) );
        EXPECT_NO_FATAL_FAILURE( netConnect.Shutdown( ) );

        EXPECT_NO_FATAL_FAILURE( network.Shutdown( ) );

//	CLogConsole::SetDisplayOmittable( false );
}

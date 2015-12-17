#include "cloginserver.h"
#include "cloginisc.h"
#include "cloginclient.h"
#include "ePacketType.h"

CLoginClient::CLoginClient() : CRoseClient(), m_Right( 0 )
{
	m_Log.SetIdentity( "CLoginClient" );
}

CLoginClient::CLoginClient( tcp::socket _sock ) : CRoseClient( std::move( _sock ) ), m_Right( 0 )
{
	m_Log.SetIdentity( "CLoginClient" );
}

void CLoginClient::SendLoginReply( uint8_t Result )
{
	CPacket* pak = new CPacket( ePacketType::PAKLC_LOGIN_REPLY, sizeof(pakLoginReply) + 1 );
	pak->pLoginReply.Result = Result;
	pak->pLoginReply.Right = m_Right;
	pak->pLoginReply.Type = 0;

	if( Result == 0 )
	{
		std::string serverName = "abcdefgh";
		uint32_t serverID = 1;

		// loop the server list here
		// TODO:: Make it so we can get the server list from the server class without holding a ptr to the server
		//TODO:: LOCK THE LIST!!!!!!! We don't want to walk through the list while servers are being added..
		std::lock_guard< std::mutex > lock( CLoginServer::GetISCListMutex() );
		for( auto& server : CLoginServer::GetISCList() )
		{
			if( server->GetType() == 1 )
			{
				pak->AddString( serverName.c_str(), true );
		                pak->Add< uint32_t >( serverID++ );
			}
		}
		pak->AddString( serverName.c_str(), true );
                pak->Add< uint32_t >( serverID + 1 );
	}
	this->Send( pak );
}

bool CLoginClient::UserLogin( CPacket* P )
{
	P->GetBytes( 0, 32, _pass );
	_pass[32] = 0; // Null term the string
	P->GetString( 32, 16, (char*)_user );
	//uint8_t* _user = P->GetString( 32, 16 );
	//uint8_t* _pass = P->GetString( 0, 32 );

	m_Log.oicprintf( "%s / %s\n", _user, _pass );

	// Query the DB
// 	{
// 		// Already logged in
// 		SendLoginReply( 4 );
// 	}
//
// 	{
// 		// Servers are under inspection
// 		SendLoginReply( 1 );
// 	}

	{
		// Okay to login!!
		SendLoginReply( 0 );
	}

// 	{
// 		// Banned
// 		SendLoginReply( 5 );
// 	}
//
// 	{
// 		// Incorrect Password
// 		SendLoginReply( 3 );
// 	}
//
// 	{
// 		// Server Full
// 		SendLoginReply( 8 );
//	}
	return true;
}

bool CLoginClient::ChannelList( CPacket* P )
{
	m_Log.icprintf( "Channel List\n" );

	uint32_t ServerID = P->pChannelListReq.lServerID;

	CPacket* pak = new CPacket( ePacketType::PAKLC_CHANNEL_LIST_REPLY, sizeof(pakChannel_List) );
	pak->pChannelList.lServerID = ServerID;
	pak->pChannelList.bServerCount = 1;


	channelInfo channel;
	channel.ChannelID = 1;
	channel.pad = 0;
	channel.Status = 0;

	pak->AddBytes( (uint8_t*)&channel, sizeof(channelInfo) );
	pak->AddString( "TestChannel", true );
	this->Send( pak );

	return true;
}

bool CLoginClient::ServerSelect( CPacket* P )
{
	(void)P;
//	uint32_t serverID = P->Get<uint32_t>( 0 );
//	uint8_t channelID = P->Get<uint8_t>( 4 );
	m_Log.icprintf( "Server Select\n" );

	CPacket* pak = new CPacket( ePacketType::PAKLC_CHANNEL_LIST_REPLY );
	pak->Add<uint8_t>( 0 );
	pak->Add<uint32_t>( 0 ); // Set this to client id
	pak->Add<uint32_t>( 0 ); // Set this to the crypt seed for the server we are connecting to
	pak->AddString( "127.0.0.1", true ); // Set this to the IP address of char server
	pak->Add<uint16_t>( 29100 ); // Set this to the char server port
	this->Send( pak );

	return true;
}

bool CLoginClient::HandlePacket( uint8_t* _buffer )
{
	CPacket* pak = (CPacket*)_buffer;
	switch ( pak->Header.Command )
	{
	case ePacketType::PAKCS_CHANNEL_LIST_REQ: return ChannelList( pak );
	case ePacketType::PAKCS_SRV_SELECT_REQ: return ServerSelect( pak );
	case ePacketType::PAKCS_LOGIN_REQ: return UserLogin( pak );

	default:
		return CRoseClient::HandlePacket( _buffer );
	}
	return true;
}

bool CLoginClient::OnReceived()
{
	return CRoseClient::OnReceived();
}

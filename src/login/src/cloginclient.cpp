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
		pak->AddString( serverName.c_str(), true );
                pak->Add< uint32_t >( serverID + 1 );
	}
	this->Send( pak );
}

bool CLoginClient::UserLogin( CPacket* P )
{
	uint8_t _user[64];
	uint8_t _pass[64];

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
// 
// 		delete _pass;
// 		delete _user;
// 		return true;
// 	}
// 
// 	{
// 		// Servers are under inspection
// 		SendLoginReply( 1 );
// 
// 		delete _pass;
// 		delete _user;
// 		return true;
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
	(void)P;
	m_Log.icprintf( "Channel List\n" );

	CPacket* pak = new CPacket( ePacketType::PAKLC_CHANNEL_LIST_REPLY, sizeof(pakChannel_List) );
	pak->pChannelList.lServerID = 0;
	pak->pChannelList.bServerCount = 1;
	channelInfo channel;
	channel.ChannelID = 1;
	channel.pad = 0;
	channel.Status = 0;

	pak->AddBytes( (uint8_t*)&channel, sizeof(channelInfo) );
	pak->AddString( "TestChannel", true );
	Send( pak );

	return true;
}

bool CLoginClient::ServerSelect( CPacket* P )
{
	(void)P;
	m_Log.icprintf( "Server Select\n" );
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

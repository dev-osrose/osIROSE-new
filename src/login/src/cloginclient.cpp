#include "cloginclient.h"
#include "ePacketType.h"

CLoginClient::CLoginClient() : CRoseClient()
{
	m_Log.SetIdentity( "CLoginClient" );
}

CLoginClient::CLoginClient( tcp::socket _sock ) : CRoseClient( std::move( _sock ) )
{
	m_Log.SetIdentity( "CLoginClient" );
}

bool CLoginClient::UserLogin( CPacket* P )
{
	uint8_t* _user = P->GetString( 32, 16 );
	uint8_t* _pass = P->GetString( 0, 32 );

	m_Log.oicprintf( "%s / %s\n", _user, _pass );

	// Query the DB
	CPacket* pak = new CPacket( ePacketType::PAKLC_LOGIN_REPLY, sizeof(pakLoginReply) );
	
// 	{
// 		// Already logged in
// 		pak->pLoginReply.Result = 4;
// 		pak->pLoginReply.Right = 0;
// 		pak->pLoginReply.Unknown = 0;
// 		Send( pak );
// 
// 		delete _pass;
// 		delete _user;
// 		return true;
// 	}
// 
// 	{
// 		// Servers are under inspection
// 		pak->pLoginReply.Result = 1;
// 		pak->pLoginReply.Right = 0;
// 		pak->pLoginReply.Unknown = 0;
// 		Send( pak );
// 
// 		delete _pass;
// 		delete _user;
// 		return true;
// 	}

	{
		// Okay to login!!
		pak->pLoginReply.Result = 0;
		pak->pLoginReply.Right = 100;
		pak->pLoginReply.Unknown = 0;

		pak->AddString( "Test123", true );
		pak->Add< uint32_t >( 1 );

		Send( pak );
	}

// 	{
// 		// Banned
// 		pak->pLoginReply.Result = 5;
// 		pak->pLoginReply.Right = 0;
// 		pak->pLoginReply.Unknown = 0;
// 		Send( pak );
// 	}
// 
// 	{
// 		// Incorrect Password
// 		pak->pLoginReply.Result = 3;
// 		pak->pLoginReply.Right = 0;
// 		pak->pLoginReply.Unknown = 0;
// 		Send( pak );
// 	}
// 
// 	{
// 		// Server Full
// 		pak->pLoginReply.Result = 8;
// 		pak->pLoginReply.Right = 0;
// 		pak->pLoginReply.Unknown = 0;
// 		Send( pak );
// 	}

	delete _pass;
	delete _user;
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

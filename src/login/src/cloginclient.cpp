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

bool CLoginClient::HandlePacket( uint8_t* _buffer )
{
	CPacket* pak = (CPacket*)_buffer;
	switch ( pak->Header.Command )
	{
	case ePacketType::PAKCS_SRV_SELECT_REQ:
	{
		break;
	}
	case ePacketType::PAKCS_LOGOUT_REQ:
	{
		break;
	}
	case ePacketType::PAKCS_LOGIN_REQ:
	{
		break;
	}
	case ePacketType::PAKCS_ACCEPT_REQ:
	{
		break;
	}
	default:
	{
		return CRoseClient::HandlePacket( _buffer );
	}
	}
	return true;
}

bool CLoginClient::OnReceived()
{
	return CRoseClient::OnReceived();
}

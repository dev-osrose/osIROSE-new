#include "ccharserver.h"
#include "ccharisc.h"
#include "ccharclient.h"
#include "ePacketType.h"

CCharClient::CCharClient() : CRoseClient(), m_Right( 0 )
{
        m_Log.SetIdentity( "CCharClient" );
}

CCharClient::CCharClient( tcp::socket _sock ) : CRoseClient( std::move( _sock ) ), m_Right( 0 )
{
        m_Log.SetIdentity( "CCharClient" );
}

bool CCharClient::HandlePacket( uint8_t* _buffer )
{
        CPacket* pak = (CPacket*)_buffer;
        switch ( pak->Header.Command )
        {
		case ePacketType::PAKCS_JOIN_SERVER_REQ: return JoinServerReply( pak ); // Allow client to connect
		case ePacketType::PAKCS_CHAR_LIST_REQ: return SendCharListReply( pak ); // SendCharList( pak );
		case ePacketType::PAKCS_CREATE_CHAR_REQ: return SendCharCreateReply( pak );
		case ePacketType::PAKCS_DELETE_CHAR_REQ: return SendCharDeleteReply( pak );
		case ePacketType::PAKCS_SELECT_CHAR_REQ: return SendCharSelectReply( pak );
        default: return CRoseClient::HandlePacket( _buffer );
        }
        return true;
}

bool CCharClient::OnReceived()
{
        return CRoseClient::OnReceived();
}

bool CCharClient::JoinServerReply( CPacket* P )
{
        (void)P;
	m_Log.icprintf( "JoinServerReply\n" );

	return true;
}

bool CCharClient::SendCharListReply( CPacket* P )
{
	(void)P;
	m_Log.icprintf( "CharListReply\n" );

	return true;
}

bool CCharClient::SendCharCreateReply( CPacket* P )
{
        (void)P;
	m_Log.icprintf( "CharCreateReply\n" );

	return true;
}

bool CCharClient::SendCharDeleteReply( CPacket* P )
{
        (void)P;
	m_Log.icprintf( "CharDeleteReply\n" );

	return true;
}

bool CCharClient::SendCharSelectReply( CPacket* P )
{
        (void)P;
	m_Log.icprintf( "CharSelectReply\n" );

	return true;
}

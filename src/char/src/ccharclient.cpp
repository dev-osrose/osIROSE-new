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
	m_Log.icprintf( "JoinServerReply\n" );

	SetId( P->Get<uint32_t>(0) );
	uint8_t _pass[33];
	P->GetBytes( 4, 32, _pass );
	_pass[ 32 ] = 0;

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
        //TODO: Find out what byte position 0 is used for.
	m_Log.icprintf( "CharDeleteReply\n" );
	uint8_t action =  P->Get<uint8_t>( 1 );
	char name[17];
	memset( name, 0, 17 );

	P->GetString( 0, 16, name );

	switch ( action )
	{
	case 0:
	case 1:
	default:
		break;
	}

	return true;
}

bool CCharClient::SendCharSelectReply( CPacket* P )
{
        (void)P;
	m_Log.icprintf( "CharSelectReply\n" );

	return true;
}

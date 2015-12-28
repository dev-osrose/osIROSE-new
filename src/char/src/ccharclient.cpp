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
	case PAKCS_JOIN_SERVER_REQ: return true; // Allow client to connect
	case PAKCS_CHAR_LIST_REQ: return true; // SendCharList( pak );
	case PAKCS_CREATE_CHAR_REQ: return true;
	case PAKCS_DELETE_CHAR_REQ: return true;
	case PAKCS_SELECT_CHAR_REQ: return true;
        default: return CRoseClient::HandlePacket( _buffer );
        }
        return true;
}

bool CCharClient::OnReceived()
{
        return CRoseClient::OnReceived();
}

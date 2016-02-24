#include "cmapserver.h"
#include "cmapisc.h"
#include "cmapclient.h"
#include "ePacketType.h"

CMapClient::CMapClient() : CRoseClient(), access_rights_( 0 )
{
        log_.SetIdentity( "CMapClient" );
}

CMapClient::CMapClient( tcp::socket _sock ) : CRoseClient( std::move( _sock ) ), access_rights_( 0 )
{
        log_.SetIdentity( "CMapClient" );
}

bool CMapClient::HandlePacket( uint8_t* _buffer )
{
        CPacket* pak = (CPacket*)_buffer;
        switch ( pak->Header.Command )
        {
//		case ePacketType::PAKCS_JOIN_SERVER_REQ: return JoinServerReply( pak ); // Allow client to connect
//		case ePacketType::PAKCS_CHAR_LIST_REQ: return SendCharListReply( pak ); // SendCharList( pak );
        default: return CRoseClient::HandlePacket( _buffer );
        }
        return true;
}

bool CMapClient::OnReceived()
{
        return CRoseClient::OnReceived();
}

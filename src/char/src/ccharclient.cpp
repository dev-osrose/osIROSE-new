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
        default: return CRoseClient::HandlePacket( _buffer );
        }
        return true;
}

bool CCharClient::OnReceived()
{
        return CRoseClient::OnReceived();
}

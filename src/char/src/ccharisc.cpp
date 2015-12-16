#include "ccharisc.h"
#include "ePacketType.h"

CCharISC::CCharISC() : CRoseISC()
{
        m_Log.SetIdentity( "CCharISC" );
}

CCharISC::CCharISC( tcp::socket _sock ) : CRoseISC( std::move( _sock ) )
{
        m_Log.SetIdentity( "CCharISC" );
}

bool CCharISC::HandlePacket( uint8_t* _buffer )
{
        CPacket* pak = (CPacket*)_buffer;
        switch ( pak->Header.Command )
        {
	case ePacketType::ISC_ALIVE: return true;
	case ePacketType::ISC_SERVER_AUTH: return true;
	case ePacketType::ISC_SERVER_REGISTER: return true;
	case ePacketType::ISC_TRANSFER: return true;
	case ePacketType::ISC_SHUTDOWN: return true;
        default:
        {
                CRoseISC::HandlePacket( _buffer );
                return false;
        }
        }
        return true;
}


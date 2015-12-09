#include "cloginisc.h"
#include "ePacketType.h"

CLoginISC::CLoginISC() : CRoseISC()
{
}

CLoginISC::CLoginISC( tcp::socket _sock ) : CRoseISC( std::move( _sock ) )
{
}

bool CLoginISC::HandlePacket( uint8_t* _buffer )
{
        CPacket* pak = (CPacket*)_buffer;
        switch( pak->Header.Command )
        {
        default:
                {
                        CRoseISC::HandlePacket( _buffer );
                        return false;
                }
        }
        return true;
}

#include "cloginclient.h"
#include "ePacketType.h"

CLoginClient::CLoginClient() : CRoseClient()
{
}

CLoginClient::CLoginClient( tcp::socket _sock ) : CRoseClient( std::move( _sock ) )
{
}

bool CLoginClient::HandlePacket( uint8_t* _buffer )
{
        CPacket* pak = (CPacket*)_buffer;
        switch( pak->Header.Command )
        {
        default:
                {
                        CRoseClient::HandlePacket( _buffer );
                        return false;
                }
        }
        return true;
}


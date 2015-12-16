#ifndef __CCHARCLIENT_H__
#define __CCHARCLIENT_H__

#include "croseclient.h"

struct CPacket;

class CCharClient : public CRoseClient
{
public:
        CCharClient( );
        CCharClient( tcp::socket _sock );

protected:
        virtual bool HandlePacket( uint8_t* _buffer );
        virtual bool OnReceived( );

        uint16_t m_Right;
};

#endif

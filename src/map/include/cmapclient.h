#ifndef __CMAPCLIENT_H__
#define __CMAPCLIENT_H__

#include "croseclient.h"

struct CPacket;

class CMapClient : public CRoseClient
{
public:
        CMapClient( );
        CMapClient( tcp::socket _sock );

protected:
        virtual bool HandlePacket( uint8_t* _buffer );
        virtual bool OnReceived( );

	uint16_t m_Right;
};

#endif

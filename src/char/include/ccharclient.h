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

		bool JoinServerReply( CPacket* P );
		bool SendCharListReply  ( CPacket* P );
		bool SendCharCreateReply( CPacket* P );
		bool SendCharDeleteReply( CPacket* P );
		bool SendCharSelectReply( CPacket* P );

        uint16_t m_Right;
};

#endif

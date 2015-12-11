#ifndef _CROSEISC_H_
#define _CROSEISC_H_

#include "croseclient.h"

class CRoseISC : public CRoseClient
{
public:
	CRoseISC();
	CRoseISC( tcp::socket _sock );
	virtual ~CRoseISC();
protected:
	// Override the callback functions we will use only
//	virtual bool OnConnect( );
        virtual void OnConnected( );
        virtual bool OnDisconnect( );
        virtual void OnDisconnected( );
//        virtual bool OnReceive( );
        virtual bool OnReceived( );
        virtual bool OnSend( uint8_t* _buffer );
        virtual void OnSent( );
	virtual bool HandlePacket( uint8_t* _buffer );
};

#endif

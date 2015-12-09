#ifndef _CROSECLIENT_H_
#define _CROSECLIENT_H_

#include "crosecrypt.h"
#include "cnetwork_asio.h"

class CRoseClient : public CNetwork_Asio
{
public:
	CRoseClient();
	CRoseClient( tcp::socket _sock );
	virtual ~CRoseClient();
protected:
	// Callback functions
        virtual bool OnConnect( );
        virtual void OnConnected( );
        virtual bool OnDisconnect( );
        virtual void OnDisconnected( );
        virtual bool OnReceive( );
        virtual void OnReceived( uint8_t* _buffer, uint16_t _size );
        virtual bool OnSend( uint8_t* _buffer );
        virtual void OnSent( );
	virtual bool HandlePacket( uint8_t* _buffer );

	PacketCodec m_Crypt;
};

#endif

#ifndef _CROSE_NETWORK_H_
#define _CROSE_NETWORK_H_

#include "cnetwork_asio.h"

class CRoseClient;

class CRoseServer : public CNetwork_Asio
{
public:
	CRoseServer( );
	virtual ~CRoseServer( );

private:
	// Callback functions
	virtual bool OnConnect( );
	virtual void OnConnected( );
	virtual bool OnListen( );
	virtual void OnListening( );
	virtual bool OnDisconnect( );
	virtual void OnDisconnected( );
	virtual bool OnReceive( );
	virtual void OnReceived( uint8_t* _buffer, uint16_t _size );
	virtual bool OnSend( uint8_t* _buffer );
	virtual void OnSent( );
	virtual bool OnAccept( );
	virtual void OnAccepted( tcp::socket _sock );

	std::list< CRoseClient* > m_ClientList;
};

#endif

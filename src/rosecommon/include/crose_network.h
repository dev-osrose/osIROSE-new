#ifndef _CROSE_NETWORK_H_
#define _CROSE_NETWORK_H_

#include "cnetwork_asio.h"

class CRose_Network : public CNetwork_Asio
{
	public:
	CRose_Network( );
	virtual ~CRose_Network( );

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
};

#endif

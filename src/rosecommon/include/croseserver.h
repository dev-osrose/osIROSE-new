#ifndef _CROSE_NETWORK_H_
#define _CROSE_NETWORK_H_

#include <list>
#include "cnetwork_asio.h"

class CRoseClient;
class CRoseISC;

class CRoseServer : public CNetwork_Asio
{
public:
	CRoseServer( bool _iscServer = false );
	virtual ~CRoseServer( );

	bool IsISCServer() { return m_ISCServer; }

private:
	// Callback functions
	virtual bool OnConnect( );
	virtual void OnConnected( );
	virtual bool OnListen( );
	virtual void OnListening( );
	virtual bool OnDisconnect( );
	virtual void OnDisconnected( );
//	virtual bool OnReceive( );
//	virtual void OnReceived( uint8_t* _buffer, uint16_t _size );
//	virtual bool OnSend( uint8_t* _buffer );
//	virtual void OnSent( );
	virtual bool OnAccept( );
	virtual void OnAccepted( tcp::socket _sock );

	bool m_ISCServer;
	std::list< CRoseClient* > m_ClientList;
	std::mutex m_ClientListMutex;
};

#endif

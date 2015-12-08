#include <stdint.h>
#include "logconsole.h"
#include "croseserver.h"
#include "croseclient.h"

CRose_Network::CRose_Network( )
{
}

CRose_Network::~CRose_Network( )
{
}

bool CRose_Network::OnConnect( )
{
	return true;
}

void CRose_Network::OnConnected( )
{
}

bool CRose_Network::OnListen( )
{
	return true;
}

void CRose_Network::OnListening( )
{
}

bool CRose_Network::OnDisconnect( )
{
	return true;
}

void CRose_Network::OnDisconnected( )
{
}

bool CRose_Network::OnReceive( )
{
	return true;
}

void CRose_Network::OnReceived( uint8_t* _buffer, uint16_t _size )
{
	(void)_buffer;
	(void)_size;
}

bool CRose_Network::OnSend( uint8_t* _buffer )
{
	(void)_buffer;
	return true;
}

void CRose_Network::OnSent( )
{
}

bool CRose_Network::OnAccept( )
{
	return true;
}

void CRose_Network::OnAccepted( tcp::socket _sock )
{
	if ( _sock.is_open( ) )
	{
		//Do Something?
		//CRoseClient* nClient = new CRoseClient( std::move (_sock) );
		//TODO:: Add the new client to the client list!
	}
}

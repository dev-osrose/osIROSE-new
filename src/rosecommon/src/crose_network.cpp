#include <stdint.h>
#include "crose_network.h"

CRose_Network::CRose_Network()
{
}

CRose_Network::~CRose_Network()
{
}

bool CRose_Network::OnConnect( )
{
	return true;
}

bool CRose_Network::OnConnected( )
{
	return true;
}

bool CRose_Network::OnListen( )
{
	return true;
}

bool CRose_Network::OnListening( )
{
	return true;
}

bool CRose_Network::OnDisconnect( )
{
	return true;
}

bool CRose_Network::OnDisconnected( )
{
	return true;
}

bool CRose_Network::OnReceive( )
{
	return true;
}

bool CRose_Network::OnReceived( uint8_t* _buffer, uint16_t _size )
{
	(void)_buffer;
	(void)_size;
	return true;
}

bool CRose_Network::OnSend( uint8_t* _buffer, uint16_t _size )
{
	(void)_buffer;
	(void)_size;
	return true;
}

bool CRose_Network::OnSent( )
{
	return true;
}

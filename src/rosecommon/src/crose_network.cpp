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

bool CRose_Network::OnSend( uint8_t* _buffer, uint16_t _size )
{
	(void)_buffer;
	(void)_size;
	return true;
}

void CRose_Network::OnSent( )
{
}

bool CRose_Network::OnAccept()
{
	return true;
}

void CRose_Network::OnAccepted(tcp::socket _sock)
{
        if( _sock.is_open() )
        {
                //Do Something?
        }
}

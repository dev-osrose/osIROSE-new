#include "croseclient.h"

CRoseClient::CRoseClient( ) : CNetwork_Asio(), m_Crypt()
{
}

CRoseClient::~CRoseClient( )
{
}

 // Callback functions
bool CRoseClient::OnConnect( )
{
	return true;
}

void CRoseClient::OnConnected( )
{
}

bool CRoseClient::OnListen( )
{
	return true;
}

void CRoseClient::OnListening( )
{
}

bool CRoseClient::OnDisconnect( )
{
	return true;
}

void CRoseClient::OnDisconnected( )
{
}

bool CRoseClient::OnReceive( )
{
	return true;
}

void CRoseClient::OnReceived( uint8_t* _buffer, uint16_t _size )
{
	//decrypt packet now
	m_Crypt.decodeClientHeader( _buffer );
	
	if( (uint16_t)_buffer[0] >= _size )
	{
		m_Crypt.decodeClientBody( _buffer );
	}

	//HandlePacket( _buffer );
}

bool CRoseClient::OnSend( uint8_t* _buffer )
{
	m_Crypt.encodeServerPacket( _buffer );
	return true;
}
void CRoseClient::OnSent( )
{
}

bool CRoseClient::OnAccept( )
{
	return true;
}

void CRoseClient::OnAccepted( tcp::socket _sock )
{
	SetSocket( std::move(_sock) );
}

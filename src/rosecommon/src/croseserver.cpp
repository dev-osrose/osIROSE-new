#include <stdint.h>
#include "logconsole.h"
#include "croseserver.h"
#include "croseclient.h"

CRoseServer::CRoseServer( )
{
	m_Log.SetIdentity( "CRoseServer" );
}

CRoseServer::~CRoseServer( )
{
}

bool CRoseServer::OnConnect( )
{
	return true;
}

void CRoseServer::OnConnected( )
{
}

bool CRoseServer::OnListen( )
{
	return true;
}

void CRoseServer::OnListening( )
{
}

bool CRoseServer::OnDisconnect( )
{
	return true;
}

void CRoseServer::OnDisconnected( )
{
}

bool CRoseServer::OnReceive( )
{
	return true;
}

void CRoseServer::OnReceived( uint8_t* _buffer, uint16_t _size )
{
	(void)_buffer;
	(void)_size;
}

bool CRoseServer::OnSend( uint8_t* _buffer )
{
	CNetwork_Asio::OnSend( _buffer );
	return true;
}

void CRoseServer::OnSent( )
{
}

bool CRoseServer::OnAccept( )
{
	return true;
}

void CRoseServer::OnAccepted( tcp::socket _sock )
{
	if ( _sock.is_open( ) )
	{
		//Do Something?
		std::string _address = _sock.remote_endpoint( ).address( ).to_string( );
		CRoseClient* nClient = new CRoseClient( std::move( _sock ) );
		m_Log.icprintf( CL_RESET CL_WHITE "Client connected from: %s\n" CL_RESET, _address.c_str( ) );
		m_ClientList.push_back( nClient );
	}
}

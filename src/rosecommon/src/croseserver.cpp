#include <stdint.h>
#include "logconsole.h"
#include "croseserver.h"
#include "croseclient.h"
#include "croseisc.h"

CRoseServer::CRoseServer( bool _iscServer ) : m_ISCServer( _iscServer )
{
	m_Log.SetIdentity( "CRoseServer" );
}

CRoseServer::~CRoseServer( )
{
	std::lock_guard<std::mutex> lock(m_ClientListMutex);
	//for(uint32_t idx = 0; idx < m_ClientList.size(); ++idx)
	//	delete m_ClientList;
	for (auto &client : m_ClientList)
	{
		client->Shutdown();
		delete client;
	}

	m_ClientList.clear();
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

/*bool CRoseServer::OnReceive( )
{
	return true;
}

void CRoseServer::OnReceived( uint8_t* _buffer, uint16_t _size )
{
	//Decrypt buffer here
	//Check header to see if we have the full packet.
	//If header.Size < _size we have more then one packet in the buffer.
	CNetwork_Asio::OnReceived( _buffer, _size );
}

bool CRoseServer::OnSend( uint8_t* _buffer )
{
	//Encrypt buffer here
	CNetwork_Asio::OnSend( _buffer );
	return true;
}

void CRoseServer::OnSent( )
{
}*/

bool CRoseServer::OnAccept( )
{
	return true;
}

void CRoseServer::OnAccepted( tcp::socket _sock )
{
	if ( _sock.is_open( ) )
	{
		//Do Something?
		std::lock_guard<std::mutex> lock(m_ClientListMutex);
		std::string _address = _sock.remote_endpoint( ).address( ).to_string( );
		CRoseClient* nClient = nullptr;
		if( IsISCServer() == false )
		{
			nClient = new CRoseClient( std::move( _sock ) );
		}
		else
		{
			nClient = new CRoseISC( std::move( _sock ) );
		}

		m_Log.icprintf( "Client connected from: %s\n", _address.c_str( ) );
		m_ClientList.push_back( nClient );
	}
}

#include "cloginserver.h"
#include "cloginclient.h"
#include "cloginisc.h"
#include "ePacketType.h"

CLoginServer::CLoginServer( bool _isc ) : CRoseServer( _isc )
{
	if ( true == _isc )
		m_Log.SetIdentity( "CLoginISCServer" );
	else
		m_Log.SetIdentity( "CLoginServer" );
}

CLoginServer::~CLoginServer()
{
}

void CLoginServer::OnAccepted( tcp::socket _sock )
{
//	CRoseServer::OnAccepted( std::move(_sock) );
	if ( _sock.is_open( ) )
	{
		//Do Something?
		std::lock_guard<std::mutex> lock(m_ClientListMutex);
		std::string _address = _sock.remote_endpoint( ).address( ).to_string( );
		CRoseClient* nClient = nullptr;
		if( IsISCServer() == false )
		{
			nClient = new CLoginClient( std::move( _sock ) );
		}
		else
		{
			nClient = new CLoginISC( std::move( _sock ) );
		}

		m_Log.icprintf( "Client connected from: %s\n", _address.c_str( ) );
		m_ClientList.push_back( nClient );
	}
}

bool CLoginServer::HandlePacket( uint8_t* _buffer )
{
	CPacket* pak = (CPacket*)_buffer;
	switch ( pak->Header.Command )
	{
	default:
	{
		CRoseServer::HandlePacket( _buffer );
		return false;
	}
	}
	return true;
}

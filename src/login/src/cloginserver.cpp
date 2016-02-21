#include "cloginserver.h"
#include "cloginclient.h"
#include "cloginisc.h"
#include "ePacketType.h"

CLoginServer::CLoginServer( bool _isc ) : CRoseServer( _isc ), clientCount(0)
{
	if ( true == _isc )
		m_Log.SetIdentity( "CLoginISCServer" );
	else
		m_Log.SetIdentity( "CLoginServer" );
}

CLoginServer::~CLoginServer()
{
	Shutdown();
}

void CLoginServer::OnAccepted( tcp::socket _sock )
{
	if ( _sock.is_open( ) )
	{
		//Do Something?
		std::string _address = _sock.remote_endpoint( ).address( ).to_string( );
		if( IsISCServer() == false )
		{
			std::lock_guard<std::mutex> lock(m_ClientListMutex);
			CLoginClient* nClient = new CLoginClient( std::move( _sock ) );
			nClient->SetId(clientCount++);
			m_Log.icprintf( "Client connected from: %s\n", _address.c_str( ) );
	                m_ClientList.push_front( nClient );
		}
		else
		{
			std::lock_guard<std::mutex> lock(m_ISCListMutex);
			CLoginISC* nClient = new CLoginISC( std::move( _sock ) );
			m_Log.icprintf( "Server connected from: %s\n", _address.c_str( ) );
                        m_ISCList.push_front( nClient );
		}
	}
}

//bool CLoginServer::HandlePacket( uint8_t* _buffer )
//{
//	CPacket* pak = (CPacket*)_buffer;
//	switch ( pak->Header.Command )
//	{
//	default:
//	{
//		CRoseServer::HandlePacket( _buffer );
//		return false;
//	}
//	}
//	return true;
//}

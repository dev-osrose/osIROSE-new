#include "ccharserver.h"
#include "ccharclient.h"
#include "ccharisc.h"
#include "ePacketType.h"

CCharServer::CCharServer( bool _isc ) : CRoseServer( _isc )
{
        if ( true == _isc )
                m_Log.SetIdentity( "CCharISCServer" );
        else
                m_Log.SetIdentity( "CCharServer" );
}

CCharServer::~CCharServer()
{
}

void CCharServer::OnAccepted( tcp::socket _sock )
{
//      CRoseServer::OnAccepted( std::move(_sock) );
        if ( _sock.is_open( ) )
        {
                //Do Something?
                std::lock_guard<std::mutex> lock(m_ClientListMutex);
                std::string _address = _sock.remote_endpoint( ).address( ).to_string( );
//                CRoseClient* nClient = nullptr;
                if( IsISCServer() == false )
                {
                        CCharClient* nClient = new CCharClient( std::move( _sock ) );
			m_Log.icprintf( "Client connected from: %s\n", _address.c_str( ) );
	                m_ClientList.push_front( nClient );
                }
                else
                {
                        CCharISC* nClient = new CCharISC( std::move( _sock ) );
			m_Log.icprintf( "Server connected from: %s\n", _address.c_str( ) );
                        m_ISCList.push_front( nClient );
                }

//                m_Log.icprintf( "Client connected from: %s\n", _address.c_str( ) );
//                m_ClientList.push_front( nClient );
        }
}

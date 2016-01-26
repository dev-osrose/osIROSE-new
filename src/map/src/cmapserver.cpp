#include "cmapserver.h"
#include "cmapclient.h"
#include "cmapisc.h"
#include "ePacketType.h"

CMapServer::CMapServer( bool _isc, int16_t mapidx ) : CRoseServer( _isc ), mapIDX( mapidx )
{
	if ( true == _isc )
                m_Log.SetIdentity( "CMapISCServer" );
        else
                m_Log.SetIdentity( "CMapServer" );

	if( mapidx >= 0 )
	{
		// We are a worker thread/process
		// We need to connect to the master thread/process to get data to handle
	}
	else
	{
		// We are a master/node process
		// We accept player connections and redirect their packet data to the correct worker
	}
}

CMapServer::~CMapServer()
{
}

void CMapServer::OnAccepted( tcp::socket _sock )
{
//      CRoseServer::OnAccepted( std::move(_sock) );
        if ( _sock.is_open( ) )
        {
                //Do Something?
                std::string _address = _sock.remote_endpoint( ).address( ).to_string( );
                if( IsISCServer() == false )
                {
                        std::lock_guard<std::mutex> lock(m_ClientListMutex);
                        CMapClient* nClient = new CMapClient( std::move( _sock ) );
                        m_Log.icprintf( "Client connected from: %s\n", _address.c_str( ) );
                        m_ClientList.push_front( nClient );
                }
                else
                {
                        std::lock_guard<std::mutex> lock(m_ISCListMutex);
                        CMapISC* nClient = new CMapISC( std::move( _sock ) );
                        m_Log.icprintf( "Server connected from: %s\n", _address.c_str( ) );
                        m_ISCList.push_front( nClient );
                }
        }
}

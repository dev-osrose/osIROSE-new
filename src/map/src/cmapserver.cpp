#include "cmapserver.h"
//#include "cmapclient.h"
//#include "cmapisc.h"
#include "ePacketType.h"

CMapServer::CMapServer( bool _isc ) : CRoseServer( _isc )
{
        if ( true == _isc )
                m_Log.SetIdentity( "CMapISCServer" );
        else
                m_Log.SetIdentity( "CMapServer" );
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
//                std::lock_guard<std::mutex> lock(m_ClientListMutex);
                std::string _address = _sock.remote_endpoint( ).address( ).to_string( );
//                CRoseClient* nClient = nullptr;
                if( IsISCServer() == false )
                {
                        std::lock_guard<std::mutex> lock(m_ClientListMutex);
//                        CMapClient* nClient = new CMapClient( std::move( _sock ) );
                        m_Log.icprintf( "Client connected from: %s\n", _address.c_str( ) );
//                        m_ClientList.push_front( nClient );
                }
                else
                {
                        std::lock_guard<std::mutex> lock(m_ISCListMutex);
//                        CMapISC* nClient = new CMapISC( std::move( _sock ) );
                        m_Log.icprintf( "Server connected from: %s\n", _address.c_str( ) );
//                        m_ISCList.push_front( nClient );
                }

//                m_Log.icprintf( "Client connected from: %s\n", _address.c_str( ) );
//                m_ClientList.push_front( nClient );
        }
}

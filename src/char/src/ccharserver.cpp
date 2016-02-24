#include "ccharserver.h"
#include "ccharclient.h"
#include "ccharisc.h"
#include "ePacketType.h"

CCharServer::CCharServer( bool _isc )
    : CRoseServer( _isc )
{
	if ( true == _isc )
		log_.SetIdentity( "CCharISCServer" );
	else
		log_.SetIdentity( "CCharServer" );
}

CCharServer::~CCharServer( )
{
}

void CCharServer::OnAccepted( tcp::socket _sock )
{
	if ( _sock.is_open( ) )
	{
		//Do Something?
		std::string _address = _sock.remote_endpoint( ).address( ).to_string( );
		if ( IsISCServer( ) == false )
		{
			std::lock_guard< std::mutex > lock( client_list_mutex_ );
			CCharClient* nClient = new CCharClient( std::move( _sock ) );
			log_.icprintf( "Client connected from: %s\n", _address.c_str( ) );
			client_list_.push_front( nClient );
		}
		else
		{
			std::lock_guard< std::mutex > lock( isc_list_mutex_ );
			CCharISC* nClient = new CCharISC( std::move( _sock ) );
			log_.icprintf( "Server connected from: %s\n", _address.c_str( ) );
			isc_list_.push_front( nClient );
		}
	}
}

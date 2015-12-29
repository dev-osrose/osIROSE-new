#include "cloginisc.h"
#include "crosepacket.h"

CLoginISC::CLoginISC( )
: CRoseISC( ), testServer(false)
{
	m_Log.SetIdentity( "CLoginISC" );
}

CLoginISC::CLoginISC( tcp::socket _sock )
: CRoseISC( std::move( _sock ) ), testServer( false )
{
	m_Log.SetIdentity( "CLoginISC" );
}

bool CLoginISC::HandlePacket( uint8_t* _buffer )
{
	CPacket* pak = (CPacket*)_buffer;
	switch ( pak->Header.Command )
	{
	case ePacketType::ISC_ALIVE: return true;
        case ePacketType::ISC_SERVER_AUTH: return true;
        case ePacketType::ISC_SERVER_REGISTER: return ServerRegister( pak );
        case ePacketType::ISC_TRANSFER: return true;
        case ePacketType::ISC_SHUTDOWN: return ServerShutdown( pak );
	default:
	{
		CRoseISC::HandlePacket( _buffer );
		return false;
	}
	}
	return true;
}

bool CLoginISC::ServerRegister( CPacket* P )
{
	CPacket* pak = (CPacket*)P;

	uint16_t _size = pak->Header.Size - 6;

	ServerReg pServerReg;
	if (pServerReg.ParseFromArray( pak->Data, _size ) == false) 
		return false;//m_Log.eicprintf( "Couldn't decode proto msg\n" );

	m_iType = pServerReg.type( );
	name = pServerReg.name();
	m_IpAddress = pServerReg.addr( );
	m_wPort = pServerReg.port( );

	m_Log.icprintf( "ISC Server Connected: [%s, %s, %s:%i]\n", ServerReg_ServerType_Name( pServerReg.type( ) ).c_str( ), name.c_str( ), m_IpAddress.c_str( ), m_wPort );
	return true;
}

bool CLoginISC::ServerShutdown( CPacket* P )
{
	(void)P;
	return true;
}

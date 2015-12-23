#include "cloginisc.h"
#include "crosepacket.h"

CLoginISC::CLoginISC() : CRoseISC()
{
	m_Log.SetIdentity( "CLoginISC" );
}

CLoginISC::CLoginISC( tcp::socket _sock ) : CRoseISC( std::move( _sock ) )
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
	CPacket* reg = (CPacket*)P;

	uint16_t _size = reg->Header.Size - 6;
	m_Log.icprintf("size: %i\n", _size);

	ServerReg pServerReg;
	if( pServerReg.ParseFromArray( reg->Data, _size ) == false ) // WHY DO YOU NOT WORK!!!!OMGFOAHSDH{O
		m_Log.eicprintf("Couldn't decode proto msg\n");

	type = pServerReg.type();
	name = pServerReg.name();
	address = pServerReg.addr();
	port = pServerReg.port();

	m_Log.icprintf("ISC Server Connected: [%s, %s, %s:%i]\n", ServerReg_ServerType_Name(pServerReg.type()).c_str(), name.c_str(), address.c_str(), port);
	return true;
}

bool CLoginISC::ServerShutdown( CPacket* P )
{
	(void)P;
	return true;
}

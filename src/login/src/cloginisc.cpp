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
	CRosePacket* reg = (CRosePacket*)P;
	type = reg->pServerReg.type();
	name = reg->pServerReg.name();
	address =reg->pServerReg.addr();
	port = reg->pServerReg.port();

	m_Log.icprintf("ISC Server Connected: [%s, %s, %s:%i]\n", ServerReg_ServerType_Name(reg->pServerReg.type()), name, address, port);
	return true;
}

bool CLoginISC::ServerShutdown( CPacket* P )
{
	(void)P;
	return true;
}

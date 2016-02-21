#include "cloginisc.h"
#include "crosepacket.h"

CLoginISC::CLoginISC( )
: CRoseISC( ), m_ChannelCount( 0 ), m_MinRight( 0 ), testServer( false )
{
	m_Log.SetIdentity( "CLoginISC" );
}

CLoginISC::CLoginISC( tcp::socket _sock )
: CRoseISC( std::move( _sock ) ), m_ChannelCount( 0 ), m_MinRight( 0 ), testServer( false )
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
	if ( pServerReg.ParseFromArray( pak->Data, _size ) == false )
		return false; //m_Log.eicprintf( "Couldn't decode proto msg\n" );

	int16_t _type = 0;

	_type = pServerReg.type( );

	//1 == char server
	//2 == node server
	//3 == map master server (This is the only type the login server will care about)
	//4 == map workers/threads

	//todo: replace these numbers with the actual enum name
	if ( _type == 1 )
	{
		name        = pServerReg.name( );
		m_IpAddress = pServerReg.addr( );
		m_wPort     = pServerReg.port( );
		m_MinRight	= pServerReg.accright( );
		m_iType = _type;
	}
	else if ( _type == 3 )
	{
		//todo: add channel connections here (_type == 3)
		tChannelInfo channel;
		channel.channelName = pServerReg.name( );
		channel.ChannelID = m_ChannelCount++;
		channel.MinRight = pServerReg.accright( );
		m_ChannelList.push_front( channel );
	}

	m_Log.icprintf( "ISC Server Connected: [%s, %s, %s:%i]\n", ServerReg_ServerType_Name( pServerReg.type( ) ).c_str( ), name.c_str( ), m_IpAddress.c_str( ), m_wPort );
	return true;
}

bool CLoginISC::ServerShutdown( CPacket* P )
{
	(void)P;
	return true;
}

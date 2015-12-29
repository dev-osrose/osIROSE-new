#include "croseisc.h"
#include "ePacketType.h"

CRoseISC::CRoseISC( )
    : CRoseClient( )
{
	m_Log.SetIdentity( "CRoseISC" );
	ResetBuffer( );
}

CRoseISC::CRoseISC( tcp::socket _sock )
    : CRoseClient( std::move( _sock ) )
{
	m_Log.SetIdentity( "CRoseISC" );
	ResetBuffer( );
}

CRoseISC::~CRoseISC( )
{
}

void CRoseISC::OnConnected( )
{
	// Do encryption handshake here
	CRoseClient::OnConnected( );
}

bool CRoseISC::OnDisconnect( )
{
	return true;
}

void CRoseISC::OnDisconnected( )
{
}

bool CRoseISC::OnReceived( )
{
	bool rtnVal = true;
	//m_Log.oicprintf( CL_WHITE "Size: %i\n", PacketSize );
	if ( PacketSize == 6 )
	{
		PacketSize = (uint16_t)Buffer[ 0 ];
		//m_Log.oicprintf( CL_WHITE "Size From buffer: %i\n", PacketSize );
		if ( PacketSize < 6 || PacketSize > MAX_PACKET_SIZE )
		{
			m_Log.eicprintf( "Client sent incorrect blockheader\n" );
			ResetBuffer( );
			return false;
		}

		if ( PacketSize > 6 )
			return true;
	}

	CPacket* pak = (CPacket*)&Buffer;
	m_Log.oicprintf( "Received a packet on CRoseISC: Header[%i, 0x%X]\n", pak->Header.Size, pak->Header.Command );
	rtnVal = HandlePacket( Buffer );
	ResetBuffer( );
	return rtnVal;
}

bool CRoseISC::OnSend( uint8_t* _buffer )
{
	//TODO: Encrypt the buffer here!
	(void)_buffer;
	return true;
}

void CRoseISC::OnSent( )
{
}

bool CRoseISC::HandlePacket( uint8_t* _buffer )
{
	CPacket* pak = (CPacket*)_buffer;
	switch ( pak->Header.Command )
	{
	case ePacketType::ISC_ALIVE: return true;
	default:
	{
		m_Log.eicprintf( "Unknown Packet Type: 0x%X\n", pak->Header.Command );
		return false;
	}
	}
	return true;
}

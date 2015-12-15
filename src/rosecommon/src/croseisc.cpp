#include "croseisc.h"
#include "ePacketType.h"

CRoseISC::CRoseISC( ) : CRoseClient( )
{
	m_Log.SetIdentity( "CRoseISC" );
}

CRoseISC::CRoseISC( tcp::socket _sock ) : CRoseClient( std::move(_sock) )
{
	m_Log.SetIdentity( "CRoseISC" );
	Recv();
}

CRoseISC::~CRoseISC()
{
}

void CRoseISC::OnConnected( )
{
	// Do encryption handshake here	
	Recv();
}

bool CRoseISC::OnDisconnect( )
{
	return true;
}

void CRoseISC::OnDisconnected()
{
}

bool CRoseISC::OnReceived()
{
	//if( (uint16_t)_buffer[0] <= _size )
	{
		HandlePacket( Buffer );
		ResetBuffer();
	}
	return true;
}

bool CRoseISC::OnSend( uint8_t* _buffer )
{
	//TODO: Encrypt the buffer here!
	//(void)_buffer;
	m_Crypt.encodeServerPacket( _buffer );
	return true;
}

void CRoseISC::OnSent( )
{
}

bool CRoseISC::HandlePacket( uint8_t* _buffer )
{
	CPacket* pak = (CPacket*)_buffer;
	switch( pak->Header.Command )
	{
	default:
		{
			m_Log.eicprintf( "Unknown Packet Type: 0x%X\n", pak->Header.Command );
			return false;
		}
	}
	return true;
}

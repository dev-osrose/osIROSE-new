#include "croseisc.h"
#include "ePacketType.h"

CRoseISC::CRoseISC( ) : CRoseClient( )
{
	m_Log.SetIdentity( "CRoseISC" );
}

CRoseISC::CRoseISC( tcp::socket _sock ) : CRoseClient( std::move(_sock) )
{
        m_Log.SetIdentity( "CRoseISC" );
}

CRoseISC::~CRoseISC()
{
}

void CRoseISC::OnConnected( )
{
	// Do encryption handshake here	
}

bool CRoseISC::OnDisconnect( )
{
	return true;
}

void CRoseISC::OnDisconnected()
{
}

void CRoseISC::OnReceived( uint8_t* _buffer, uint16_t _size )
{
	m_Log.oicprintf( "Received a packet on CRoseISC: _size = %i\n", _size );
	if( HandlePacket( _buffer ) == false )
	{
		//Something happened.
	}
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
	switch( pak->Header.Command )
	{
	default:
		{
			m_Log.eicprintf( "Unknown Packet Type: %i\n", pak->Header.Command );
			return false;
		}
	}
	return true;
}

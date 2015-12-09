#include "croseclient.h"
#include "ePacketType.h"

CRoseClient::CRoseClient( ) : CNetwork_Asio( ), m_Crypt( )
{
	m_Log.SetIdentity( "CRoseClient" );
}

CRoseClient::CRoseClient( tcp::socket _sock ) : CNetwork_Asio( ), m_Crypt( )
{
	SetSocket( std::move(_sock) );
	m_Log.SetIdentity( "CRoseClient" );
}

CRoseClient::~CRoseClient( )
{
}

 // Callback functions
bool CRoseClient::OnConnect( )
{
	return true;
}

void CRoseClient::OnConnected( )
{
}

bool CRoseClient::OnDisconnect( )
{
	return true;
}

void CRoseClient::OnDisconnected( )
{
}

bool CRoseClient::OnReceive( )
{
	return true;
}

void CRoseClient::OnReceived( uint8_t* _buffer, uint16_t _size )
{
	m_Log.oicprintf( "Received a packet on CRoseClient: _size = %i\n", _size );
	//decrypt packet now
	m_Crypt.decodeClientHeader( _buffer );

	if( (uint16_t)_buffer[0] <= _size )
	{
		m_Crypt.decodeClientBody( _buffer );
		HandlePacket( _buffer );
		ResetBuffer();
	}
}

bool CRoseClient::OnSend( uint8_t* _buffer )
{
	(void)_buffer;
	//m_Crypt.encodeServerPacket( _buffer );
	return true;
}
void CRoseClient::OnSent( )
{
}

bool CRoseClient::HandlePacket( uint8_t* _buffer )
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

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

bool CRoseClient::OnReceived()
{
	//uint8_t buf[MAX_PACKET_SIZE];
	//memcpy( buf, Buffer, _size );

	if (PacketSize == 6)
	{
		PacketSize = m_Crypt.decodeClientHeader( (unsigned char*)&Buffer );

		if (PacketSize < 6)
		{
			m_Log.eicprintf( "Client sent incorrect blockheader\n" );
			return false;
		}

		if (PacketSize > 6)
			return true;
	}

	//decrypt packet now
	if (!m_Crypt.decodeClientBody( (unsigned char*)&Buffer ))
	{
		// ERROR!!!
		m_Log.eicprintf( "Client sent illegal block\n"); 
		return false;
	}

	CPacket* pak = (CPacket*)&Buffer;
	m_Log.oicprintf( "Received a packet on CRoseClient: Header[%i, %i]\n", pak->Header.Size, pak->Header.Command );
	HandlePacket( Buffer );
	ResetBuffer();

	return true;
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
        switch( (ePacketType)pak->Header.Command )
        {
	case ePacketType::PAKCS_ALIVE:
	{
		m_Log.icprintf( "Got keep alive packet\n" );
		break;
	}
        default:
                {
                        m_Log.eicprintf( "Unknown Packet Type: %i\n", pak->Header.Command );
                        return false;
                }
        }
        return true;
}

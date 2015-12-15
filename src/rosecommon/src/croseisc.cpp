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
	if ( PacketSize == 6 )
        {
                PacketSize = (uint16_t)Buffer[0];//m_Crypt.decodeClientHeader( (unsigned char*)&Buffer );

                if ( PacketSize < 6 )
                {
                        m_Log.eicprintf( "Client sent incorrect blockheader\n" );
                        return false;
                }

                if ( PacketSize > 6 )
                        return true;
        }

        //decrypt packet now
//        if ( !m_Crypt.decodeClientBody( (unsigned char*)&Buffer ) )
//        {
//                // ERROR!!!
//                m_Log.eicprintf( "Client sent illegal block\n" );
//                return false;
//        }

        CPacket* pak = (CPacket*)&Buffer;
        m_Log.oicprintf( "Received a packet on CRoseISC: Header[%i, 0x%X]\n", pak->Header.Size, pak->Header.Command );
	HandlePacket( Buffer );
	ResetBuffer();
	return true;
}

bool CRoseISC::OnSend( uint8_t* _buffer )
{
	//TODO: Encrypt the buffer here!
	(void)_buffer;
	//m_Crypt.encodeServerPacket( _buffer );
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

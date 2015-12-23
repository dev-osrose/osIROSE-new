#include "ccharisc.h"
#include "crosepacket.h"

CCharISC::CCharISC() : CRoseISC()
{
        m_Log.SetIdentity( "CCharISC" );
}

CCharISC::CCharISC( tcp::socket _sock ) : CRoseISC( std::move( _sock ) )
{
        m_Log.SetIdentity( "CCharISC" );
}

bool CCharISC::HandlePacket( uint8_t* _buffer )
{
        CPacket* pak = (CPacket*)_buffer;
        switch ( pak->Header.Command )
        {
	case ePacketType::ISC_ALIVE: return true;
	case ePacketType::ISC_SERVER_AUTH: return true;
	case ePacketType::ISC_SERVER_REGISTER: return true;
	case ePacketType::ISC_TRANSFER: return true;
	case ePacketType::ISC_SHUTDOWN: return true;
        default:
        {
                CRoseISC::HandlePacket( _buffer );
                return false;
        }
        }
        return true;
}

void CCharISC::OnConnected()
{
	CRosePacket* pak = new CRosePacket(ePacketType::ISC_SERVER_REGISTER);
	ServerReg pServerReg;
	pServerReg.set_name("");
	pServerReg.set_addr("");
	pServerReg.set_port(29100);
	pServerReg.set_type(ServerReg_ServerType_CHAR);


	int size = pServerReg.ByteSize(); 
	uint8_t* data = new uint8_t[size];
	pServerReg.SerializeToArray(data, size);
	//std::ostringstream stream;
	//pServerReg.SerializeToOstream(&stream);
	//std::string data = stream.str();
	pak->AddBytes(data, size);
	Send((CPacket*)pak);

	delete[] data;
}

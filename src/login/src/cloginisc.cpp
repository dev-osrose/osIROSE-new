#include "cloginisc.h"
#include "crosepacket.h"

CLoginISC::CLoginISC()
    : CRoseISC(), channel_count_(1), min_right_(0), test_server_(false) {
  log_.SetIdentity("CLoginISC");
}

CLoginISC::CLoginISC(tcp::socket _sock)
    : CRoseISC(std::move(_sock)),
      channel_count_(1),
      min_right_(0),
      test_server_(false) {
  log_.SetIdentity("CLoginISC");
}

bool CLoginISC::HandlePacket(uint8_t* _buffer) {
  CRosePacket* pak = (CRosePacket*)_buffer;
  switch (pak->Header.Command) {
    case ePacketType::ISC_ALIVE:
      return true;
    case ePacketType::ISC_SERVER_AUTH:
      return true;
    case ePacketType::ISC_SERVER_REGISTER:
      return ServerRegister(pak);
    case ePacketType::ISC_TRANSFER:
      return true;
    case ePacketType::ISC_SHUTDOWN:
      return ServerShutdown(pak);
    default: {
      CRoseISC::HandlePacket(_buffer);
      return false;
    }
  }
  return true;
}

bool CLoginISC::ServerRegister(CRosePacket* P) {
  CRosePacket* pak = (CRosePacket*)P;

  uint16_t _size = pak->Header.Size - 6;

  ServerReg pServerReg;
  if (pServerReg.ParseFromArray(pak->Data, _size) == false)
    return false;  // m_Log.eicprintf( "Couldn't decode proto msg\n" );

  int16_t _type = 0;

  _type = pServerReg.type();

  // 1 == char server
  // 2 == node server
  // 3 == map master server (This is the only type the login server will care
  // about)
  // 4 == map workers/threads

  // todo: replace these numbers with the actual enum name
  if (_type == 1) {
    server_name_ = pServerReg.name();
    network_ip_address = pServerReg.addr();
    network_port_ = pServerReg.port();
    min_right_ = pServerReg.accright();
    network_type_ = _type;
  } else if (_type == 3) {
    // todo: add channel connections here (_type == 3)
    tChannelInfo channel;
    channel.channelName = pServerReg.name();
    channel.ChannelID = channel_count_++;
    channel.MinRight = pServerReg.accright();
    channel_list_.push_front(channel);
  }

  log_.icprintf("ISC Server Connected: [%s, %s, %s:%i]\n",
                ServerReg_ServerType_Name(pServerReg.type()).c_str(),
                server_name_.c_str(), network_ip_address.c_str(),
                network_port_);
  return true;
}

bool CLoginISC::ServerShutdown(CRosePacket* P) {
  (void)P;
  return true;
}

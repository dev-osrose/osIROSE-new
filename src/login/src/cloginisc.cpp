#include "cloginisc.h"
#include "crosepacket.h"
#include "iscpackets.pb.h"

using namespace RoseCommon;

CLoginISC::CLoginISC()
    : CRoseISC(), channel_count_(1), min_right_(0), test_server_(false) {}

CLoginISC::CLoginISC(tcp::socket _sock)
    : CRoseISC(std::move(_sock)),
      channel_count_(1),
      min_right_(0),
      test_server_(false) {}

bool CLoginISC::HandlePacket(uint8_t* _buffer) {
  switch (CRosePacket::type(_buffer)) {
    case ePacketType::ISC_ALIVE:
      return true;
    case ePacketType::ISC_SERVER_AUTH:
      return true;
    case ePacketType::ISC_SERVER_REGISTER:
      return ServerRegister(CRosePacket(_buffer));
    case ePacketType::ISC_TRANSFER:
      return true;
    case ePacketType::ISC_SHUTDOWN:
      return ServerShutdown(CRosePacket(_buffer));
    default: { return CRoseISC::HandlePacket(_buffer); }
  }
  return true;
}

bool CLoginISC::ServerRegister(const CRosePacket& P) {
  uint16_t _size = P.size() - 6;

  iscPacket::ServerReg pServerReg;
  if (pServerReg.ParseFromArray(const_cast<CRosePacket&>(P).data(), _size) ==
      false)
    return false;  // m_Log.eicprintf( "Couldn't decode proto msg\n" );

  int16_t _type = 0;

  _type = pServerReg.type();

  // 1 == char server
  // 2 == node server
  // 3 == map master server (This is the only type the login server will care
  // about)
  // 4 == map workers/threads

  // todo: replace these numbers with the actual enum name
  if (_type == iscPacket::ServerType::CHAR) {
    server_name_ = pServerReg.name();
    network_ip_address = pServerReg.addr();
    network_port_ = pServerReg.port();
    min_right_ = pServerReg.accright();
    network_type_ = _type;
  } else if (_type == iscPacket::ServerType::MAP_MASTER) {
    // todo: add channel connections here (_type == 3)
    tChannelInfo channel;
    channel.channelName = pServerReg.name();
    channel.ChannelID = pServerReg.id();
    channel.MinRight = pServerReg.accright();
    channel_list_.push_front(channel);
    channel_count_++;
  }

  logger_->notice("ISC Server Connected: [{}, {}, {}:{}]\n",
                  ServerType_Name(pServerReg.type()).c_str(),
                  pServerReg.name().c_str(), pServerReg.addr().c_str(),
                  pServerReg.port());
  return true;
}

bool CLoginISC::ServerShutdown(const CRosePacket& P) {
  (void)P;
  uint16_t _size = P.size() - 6;

  iscPacket::ServerShutdown pServerShutdown;
  if (pServerShutdown.ParseFromArray(const_cast<CRosePacket&>(P).data(),
                                     _size) == false)
    return false;

  channel_list_.remove_if([pServerShutdown](RoseCommon::tChannelInfo channel) {
    return channel.ChannelID == pServerShutdown.id();
  });
  return true;
}

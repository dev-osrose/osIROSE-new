#include "cmapisc.h"
#include "crosepacket.h"
#include "config.h"
#include "iscpackets.pb.h"
#include "rosepackets.h"

using namespace RoseCommon;

CMapISC::CMapISC() : CRoseISC(), char_server_(false) {
}

CMapISC::CMapISC(tcp::socket _sock)
    : CRoseISC(std::move(_sock)), char_server_(false) {
}

bool CMapISC::HandlePacket(uint8_t* _buffer) {
  switch (CRosePacket::type(_buffer)) { 
    case ePacketType::ISC_ALIVE: 
      return true; 
    case ePacketType::ISC_SERVER_AUTH: 
      return true; 
    case ePacketType::ISC_SERVER_REGISTER: 
      return true; 
    case ePacketType::ISC_TRANSFER: 
      return true; 
    case ePacketType::ISC_SHUTDOWN: 
      return true; 
    default: { 
      CRoseISC::HandlePacket(_buffer); 
      return false; 
    } 
  } 
  return true;
}

void CMapISC::OnConnected() {
  Core::Config& config = Core::Config::getInstance(); 
  auto packet = makePacket<ePacketType::ISC_SERVER_REGISTER>(
    config.map_server().channelname(), config.serverdata().ip(),
    config.map_server().clientport(),
    iscPacket::ServerReg_ServerType::ServerReg_ServerType_MAP_MASTER,
    config.map_server().accesslevel() );

  logger_->trace( "Sending a packet on CMapISC: Header[{0}, 0x{1:x}]",
                  packet->size(), (uint16_t)packet->type() );
  Send(*packet); 
}

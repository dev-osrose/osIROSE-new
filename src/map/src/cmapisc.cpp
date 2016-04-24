#include "cmapisc.h"
#include "crosepacket.h"
#include "config.h"
#include "iscpackets.pb.h"
#include "rosepackets.h"

using namespace RoseCommon;

CMapISC::CMapISC() : CRoseISC(), char_server_(false) {}

CMapISC::CMapISC(tcp::socket _sock)
    : CRoseISC(std::move(_sock)), char_server_(false) {}

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
      config.map_server().channelname(), config.serverdata().ip(), GetId(),
      config.map_server().clientport(), iscPacket::ServerType::MAP_MASTER,
      config.map_server().accesslevel());

  logger_->trace("Sending a packet on CMapISC: Header[{0}, 0x{1:x}]",
                 packet->size(), (uint16_t)packet->type());
  Send(*packet);

  process_thread_ = std::thread([this]() {
    while (active_ == true && char_server_ == true) {
      std::chrono::steady_clock::time_point update = Core::Time::GetTickCount();
      int64_t dt = std::chrono::duration_cast<std::chrono::milliseconds>(
                       update - GetLastUpdateTime())
                       .count();
      if (dt > (1000 * 60) * 1)  // wait 4 minutes before pinging
      {
        logger_->trace("Sending ISC_ALIVE");
        auto packet = std::unique_ptr<CRosePacket>(
            new CRosePacket(ePacketType::ISC_ALIVE));
        Send(*packet);
      }
      std::this_thread::sleep_for(
          std::chrono::milliseconds(500));  // sleep for 30 seconds
    }
    return 0;
  });
}

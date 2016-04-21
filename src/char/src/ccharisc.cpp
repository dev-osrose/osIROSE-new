#include "ccharisc.h"
#include "crosepacket.h"
#include "ccharserver.h"
#include "config.h"
#include "iscpackets.pb.h"
#include <memory>
#include "rosepackets.h"

using namespace RoseCommon;

CCharISC::CCharISC() : CRoseISC(), login_connection_(false) {}

CCharISC::CCharISC(tcp::socket _sock)
    : CRoseISC(std::move(_sock)), login_connection_(false) {}

bool CCharISC::HandlePacket(uint8_t* _buffer) {
  switch (CRosePacket::type(_buffer)) {
    case ePacketType::ISC_ALIVE:
      return true;
    case ePacketType::ISC_SERVER_AUTH:
      return true;
    case ePacketType::ISC_SERVER_REGISTER:
      return ServerRegister(
          getPacket<ePacketType::ISC_SERVER_REGISTER>(_buffer));
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

bool CCharISC::ServerRegister(
    std::unique_ptr<RoseCommon::IscServerRegister> P) {
  logger_->trace("CCharISC::ServerRegister(CRosePacket* P)");

  uint16_t _size = P->size() - 6;

  iscPacket::ServerReg pMapServer;
  if (pMapServer.ParseFromArray(P->data(), _size) == false) {
    logger_->debug("pMapServer.ParseFromArray Failed!");
    return false;
  }
  int16_t _type = 0;
  _type = pMapServer.type();

  // 1 == char server
  // 2 == node server
  // 3 == map master server (This is the only type the login server will care
  // about)
  // 4 == map workers/threads

  iscPacket::ServerReg pServerReg;
  std::string name, ip;
  int32_t port = 0, type = 0, right = 0;

  if (_type == 2) {
    // This is a node and we need to figure out something to do with this
  } else if (_type == 3) {
    name = pMapServer.name();
    ip = pMapServer.addr();
    port = pMapServer.port();
    type = pMapServer.type();
    right = pMapServer.accright();
  }

  logger_->notice("ISC Server Connected: [{}, {}, {}:{}]\n",
                  ServerReg_ServerType_Name(pMapServer.type()).c_str(),
                  pMapServer.name().c_str(), pMapServer.addr().c_str(),
                  pMapServer.port());

  auto packet =
      makePacket<ePacketType::ISC_SERVER_REGISTER>(name, ip, port, type, right);

  // todo: get the ISC connection to the login server and send the packet to it
  std::lock_guard<std::mutex> lock(CCharServer::GetISCListMutex());
  for (auto& server : CCharServer::GetISCList()) {
    CCharISC* svr = (CCharISC*)server;
    if (svr->IsLogin()) {
      svr->Send(*packet);
      return true;
    }
  }
  return false;
}

void CCharISC::OnConnected() {
  logger_->trace("CCharISC::OnConnected()");

  Core::Config& config = Core::Config::getInstance();
  auto packet = makePacket<ePacketType::ISC_SERVER_REGISTER>(
      config.char_server().worldname(), config.serverdata().ip(),
      config.char_server().clientport(),
      iscPacket::ServerReg_ServerType::ServerReg_ServerType_CHAR,
      config.char_server().accesslevel());

  logger_->trace("Sending a packet on CCharISC: Header[{0}, 0x{1:x}]",
                 packet->size(), (uint16_t)packet->type());
  Send(*packet);
}

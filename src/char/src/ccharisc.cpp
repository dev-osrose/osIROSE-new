// Copyright 2016 Chirstopher Torres (Raven), L3nn0x
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
// http ://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <memory>
#include "ccharisc.h"
#include "crosepacket.h"
#include "ccharserver.h"
#include "config.h"
#include "iscpackets.pb.h"
#include "packets.h"
#include "platform_defines.h"

using namespace RoseCommon;

CCharISC::CCharISC() : CRoseISC() {}

CCharISC::CCharISC(Core::INetwork* _sock) : CRoseISC(std::move(_sock)) {}

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
  std::string name;
  int32_t type = 0, right = 0;

  if (_type == iscPacket::ServerType::NODE) {
    // This is a node and we need to figure out something to do with this
  } else if (_type == iscPacket::ServerType::MAP_MASTER) {
    name = pMapServer.name();
    socket_->set_address(pMapServer.addr());
    socket_->set_port(pMapServer.port());
    type = pMapServer.type();
    right = pMapServer.accright();

    socket_->set_type(_type);
  }

  logger_->info("ISC Server Connected: [{}, {}, {}:{}]\n",
                  ServerType_Name(pMapServer.type()).c_str(),
                  pMapServer.name().c_str(), pMapServer.addr().c_str(),
                  pMapServer.port());

  auto packet = makePacket<ePacketType::ISC_SERVER_REGISTER>(name, socket_->get_address(), get_id(),
    socket_->get_port(), type, right);

  // todo: get the ISC connection to the login server and send the packet to
  // it
  std::lock_guard<std::mutex> lock(CCharServer::GetISCListMutex());
  for (auto& server : CCharServer::GetISCList()) {
    CCharISC* svr = (CCharISC*)server;
    if (svr->IsLogin() == true) {
      svr->send(*packet);
      return true;
    }
  }
  return false;
}

void CCharISC::OnConnected() {
  logger_->trace("CCharISC::OnConnected()");

  Core::Config& config = Core::Config::getInstance();
  auto packet = makePacket<ePacketType::ISC_SERVER_REGISTER>(
      config.char_server().worldname(), config.serverdata().ip(), get_id(),
      config.char_server().clientport(), iscPacket::ServerType::CHAR,
      config.char_server().accesslevel());

  logger_->trace("Sending a packet on CCharISC: Header[{0}, 0x{1:x}]",
                 packet->size(), (uint16_t)packet->type());
  send(*packet);

  socket_->set_type(iscPacket::ServerType::LOGIN);

  if (socket_->process_thread_.joinable() == false) {
    socket_->process_thread_ = std::thread([this]() {
		  while (is_active() == true && IsLogin() == true) {
			  std::chrono::steady_clock::time_point update = Core::Time::GetTickCount();
			  int64_t dt = std::chrono::duration_cast<std::chrono::milliseconds>(
				  update - get_update_time())
				  .count();
			  if (dt > (1000 * 60) * 1)  // wait 4 minutes before pinging
			  {
				  logger_->trace("Sending ISC_ALIVE");
				  auto packet = std::unique_ptr<CRosePacket>(
					  new CRosePacket(ePacketType::ISC_ALIVE));
				  send(*packet);
			  }
			  std::this_thread::sleep_for(
				  std::chrono::milliseconds(500));  // sleep for 30 seconds
		  }
		  return 0;
	  });
  }
}

bool CCharISC::OnShutdown() {
  logger_->trace("CCharISC::OnDisconnected()");
  bool result = true;

  if (is_active() == true) {
    if (get_type() == iscPacket::ServerType::LOGIN) {
      if (socket_->reconnect() == true) {
        logger_->info("Reconnected to login server.");
        result = false;
      }
    } else {
		auto packet = std::unique_ptr<CRosePacket>(
			new CRosePacket(ePacketType::ISC_SHUTDOWN));
      //auto packet = makePacket<ePacketType::ISC_SHUTDOWN>(get_id());
      std::lock_guard<std::mutex> lock(CCharServer::GetISCListMutex());
      for (auto& server : CCharServer::GetISCList()) {
        CCharISC* svr = (CCharISC*)server;
        if (svr->IsLogin()) {
          svr->send(*packet);
          break;
        }
      }
    }
  }
  return result;
}

bool CCharISC::IsLogin() const {
  return (get_type() == iscPacket::ServerType::LOGIN);
}

void CCharISC::SetLogin(bool val) {
  if (val == true) socket_->set_type(iscPacket::ServerType::LOGIN);
}

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
#include "isc_serverregister.h"
#include "isc_shutdown.h"

#include "platform_defines.h"

using namespace RoseCommon;

CCharISC::CCharISC() : CRoseISC() {}

CCharISC::CCharISC(std::unique_ptr<Core::INetwork> _sock) : CRoseISC(std::move(_sock)) {
  socket_->registerOnConnected(std::bind(&CCharISC::OnConnected, this));
  socket_->registerOnShutdown(std::bind(&CCharISC::OnShutdown, this));
}

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

  // 1 == char server
  // 2 == node server
  // 3 == map master server (This is the only type the login server will care
  // about)
  // 4 == map workers/threads

  std::string name;
  int32_t right = 0;
  RoseCommon::Isc::ServerType type;

  if (P->serverType() == RoseCommon::Isc::ServerType::NODE) {
    // This is a node and we need to figure out something to do with this
  } else if (P->serverType() == RoseCommon::Isc::ServerType::MAP_MASTER) {
    name = P->name();
    socket_->set_address(P->addr());
    socket_->set_port(P->port());
    type = P->serverType();
    right = P->right();

    socket_->set_type(to_underlying(type));
  }

  logger_->info("ISC Server Connected: [{}, {}, {}:{}]\n",
                RoseCommon::Isc::serverTypeName(P->serverType()),
                  P->name(), P->addr(),
                  P->port());

  auto packet = makePacket<ePacketType::ISC_SERVER_REGISTER>(type, name, socket_->get_address(),
                                                             socket_->get_port(), right, get_id());

  // todo: get the ISC connection to the login server and send the packet to
  // it
  std::lock_guard<std::mutex> lock(CCharServer::GetISCListMutex());
  for (auto& server : CCharServer::GetISCList()) {
    CCharISC* svr = static_cast<CCharISC*>(server.get());
    if (!svr) {
        continue;
    }
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
      RoseCommon::Isc::ServerType::CHAR,
      config.charServer().worldName, config.serverData().ip,
      config.charServer().clientPort,
      config.charServer().accessLevel,
      get_id());

  logger_->trace("Sending a packet on CCharISC: Header[{0}, 0x{1:x}]",
                 packet->size(), (uint16_t)packet->type());
  send(*packet);

  socket_->set_type(to_underlying(Isc::ServerType::LOGIN));

  if (socket_->process_thread_.joinable() == false) {
    socket_->process_thread_ = std::thread([this]() {
      while (this->is_active() == true && this->IsLogin() == true) {
        std::chrono::steady_clock::time_point update = Core::Time::GetTickCount();
        int64_t dt = std::chrono::duration_cast<std::chrono::milliseconds>(
          update - get_update_time())
          .count();
        if (dt > (1000 * 60) * 1)  // wait 1 minutes before pinging
        {
          logger_->trace("Sending ISC_ALIVE");
          auto packet = std::unique_ptr<CRosePacket>(
            new CRosePacket(ePacketType::ISC_ALIVE));
          send(*packet);
        }
        std::this_thread::sleep_for(
          std::chrono::milliseconds(1000));
      }
      return 0;
    });
  }
}

bool CCharISC::OnShutdown() {
  logger_->trace("CCharISC::OnShutdown()");
  bool result = true;

  if (is_active() == true) {
    if (get_type() == Isc::ServerType::LOGIN) {
      if (socket_->reconnect() == true) {
        logger_->info("Reconnected to login server.");
        result = false;
      }
    } else {
      auto packet = makePacket<ePacketType::ISC_SHUTDOWN>();
      std::lock_guard<std::mutex> lock(CCharServer::GetISCListMutex());
      for (auto& server : CCharServer::GetISCList()) {
        CCharISC* svr = static_cast<CCharISC*>(server.get());
        if (!svr) {
            continue;
        }
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
  return (get_type() == Isc::ServerType::LOGIN);
}

void CCharISC::SetLogin(bool val) {
  if (val == true) socket_->set_type(to_underlying(Isc::ServerType::LOGIN));
}

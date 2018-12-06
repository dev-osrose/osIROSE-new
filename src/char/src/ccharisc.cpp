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
#include "isc_shutdown.h"
#include "isc_alive.h"

#include "platform_defines.h"

using namespace RoseCommon;

CCharISC::CCharISC() : CRoseISC(), server_(nullptr) {}

CCharISC::CCharISC(CCharServer* server, std::unique_ptr<Core::INetwork> _sock) : CRoseISC(std::move(_sock)), server_(server) {
  socket_[SocketType::Client]->registerOnConnected(std::bind(&CCharISC::onConnected, this));
  socket_[SocketType::Client]->registerOnShutdown(std::bind(&CCharISC::onShutdown, this));
}

bool CCharISC::handlePacket(uint8_t* _buffer) {
  switch (CRosePacket::type(_buffer)) {
    case ePacketType::ISC_ALIVE:
      return true;
    case ePacketType::ISC_SERVER_AUTH:
      return true;
    case ePacketType::ISC_SERVER_REGISTER:
      return serverRegister(
          Packet::IscServerRegister::create(_buffer));
    case ePacketType::ISC_TRANSFER:
      return true;
    case ePacketType::ISC_SHUTDOWN:
      return true;
    default: {
      CRoseISC::handlePacket(_buffer);
      return false;
    }
  }
  return true;
}

bool CCharISC::serverRegister(RoseCommon::Packet::IscServerRegister&& P) {
  logger_->trace("CCharISC::serverRegister(CRosePacket P)");

  // 1 == char server
  // 2 == node server
  // 3 == map master server (This is the only type the login server will care
  // about)
  // 4 == map workers/threads

  std::string name;
  int32_t right = 0;
  RoseCommon::Isc::ServerType type = RoseCommon::Isc::ServerType::NODE;

  if (P.get_serverType() == RoseCommon::Isc::ServerType::NODE) {
    // This is a node and we need to figure out something to do with this
  } else if (P.get_serverType() == RoseCommon::Isc::ServerType::MAP_MASTER) {
    name = P.get_name();
    socket_[SocketType::Client]->set_address(P.get_addr());
    socket_[SocketType::Client]->set_port(P.get_port());
    type = P.get_serverType();
    right = P.get_right();

    socket_[SocketType::Client]->set_type(to_underlying(type));
  }

  logger_->info("ISC Server Connected: [{}, {}, {}:{}]\n",
                RoseCommon::Isc::serverTypeName(P.get_serverType()),
                  P.get_name(), P.get_addr(),
                  P.get_port());

  auto packet = Packet::IscServerRegister::create(type, name, socket_[SocketType::Client]->get_address(),
                                                             socket_[SocketType::Client]->get_port(), right, get_id());

  // todo: get the ISC connection to the login server and send the packet to
  // it
  std::lock_guard<std::mutex> lock(server_->GetISCListMutex());
  for (auto& server : server_->GetISCList()) {
    CCharISC* svr = static_cast<CCharISC*>(server.get());
    if (!svr) {
        continue;
    }
    if (svr->isLogin() == true) {
      svr->send(packet);
      return true;
    }
  }
  return false;
}

void CCharISC::onConnected() {
  logger_->trace("CCharISC::onConnected()");

  Core::Config& config = Core::Config::getInstance();
  auto packet = Packet::IscServerRegister::create(
      RoseCommon::Isc::ServerType::CHAR,
      config.charServer().worldName, config.serverData().ip,
      config.charServer().clientPort,
      config.charServer().accessLevel,
      get_id());

  send(packet);

  socket_[SocketType::Client]->set_type(to_underlying(Isc::ServerType::LOGIN));

  if (socket_[SocketType::Client]->process_thread_.joinable() == false) {
    socket_[SocketType::Client]->process_thread_ = std::thread([this]() {
      while (this->is_active() == true && this->isLogin() == true) {
        std::chrono::steady_clock::time_point update = Core::Time::GetTickCount();
        int64_t dt = std::chrono::duration_cast<std::chrono::milliseconds>(
          update - get_update_time())
          .count();
        if (dt > (1000 * 60) * 1)  // wait 1 minutes before pinging
        {
          logger_->trace("Sending ISC_ALIVE");
          auto packet = Packet::IscAlive::create();
          send(packet);
        }
        std::this_thread::sleep_for(
          std::chrono::milliseconds(1000));
      }
      return 0;
    });
  }
}

bool CCharISC::onShutdown() {
  logger_->trace("CCharISC::onShutdown()");
  bool result = true;

  if (is_active() == true) {
    if (get_type() == Isc::ServerType::LOGIN) {
      if (socket_[SocketType::Client]->reconnect() == true) {
        logger_->info("Reconnected to login server.");
        result = false;
      }
    } else {
      auto packet = RoseCommon::Packet::IscShutdown::create(get_type(), get_id());
      std::lock_guard<std::mutex> lock(server_->GetISCListMutex());
      for (auto& server : server_->GetISCList()) {
        CCharISC* svr = static_cast<CCharISC*>(server.get());
        if (!svr) {
            continue;
        }
        if (svr->isLogin()) {
          svr->send(packet);
        }
      }
    }
  }
  return result;
}

bool CCharISC::isLogin() const {
  return (get_type() == Isc::ServerType::LOGIN);
}

void CCharISC::setLogin(bool val) {
  if (val == true) socket_[SocketType::Client]->set_type(to_underlying(Isc::ServerType::LOGIN));
}

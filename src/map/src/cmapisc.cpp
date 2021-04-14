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

#include "cmapisc.h"
#include "cmapserver.h"
#include "config.h"
#include "crosepacket.h"
#include "cli_login_req.h"
#include "isc_shutdown.h"
#include "isc_alive.h"
#include "platform_defines.h"
#include "entity_system.h"

using namespace RoseCommon;

CMapISC::CMapISC() : CRoseISC(), server_(nullptr) {
  //  socket_->set_type(iscPacket::ServerType::MAP_MASTER);
}

CMapISC::CMapISC(CMapServer* server, std::unique_ptr<Core::INetwork> _sock) : CRoseISC(std::move(_sock)), server_(server) {
  socket_[SocketType::Client]->set_type(to_underlying(Isc::ServerType::MAP_MASTER));
  socket_[SocketType::Client]->registerOnConnected(std::bind(&CMapISC::onConnected, this));
  socket_[SocketType::Client]->registerOnShutdown(std::bind(&CMapISC::onShutdown, this));
}

void CMapISC::add_maps(const std::vector<uint16_t>& maps) {
    for (auto m : maps) {
        this->maps.insert({m, {}});
    }
}

void CMapISC::register_map(uint16_t map, std::weak_ptr<EntitySystem> system) {
    maps[map] = system;
}

bool CMapISC::transfer(RoseCommon::Packet::IscTransfer&& P) {
    if (P.get_maps().empty()) {
        for (auto map : maps) {
            if (auto ptr = map.second.lock()) {
                if (P.get_serverPacket()) {
                    ptr->dispatch_packet(entt::null, RoseCommon::fetchPacket<true>(static_cast<const uint8_t*>(P.get_blob().data())));
                } else {
                    ptr->dispatch_packet(entt::null, RoseCommon::fetchPacket<false>(static_cast<const uint8_t*>(P.get_blob().data())));
                }
            }
        }
    } else {
        for (auto map : P.get_maps()) {
            if (auto it = maps.find(map); it != maps.end()) {
                if (auto ptr = it->second.lock()) {
                    if (P.get_serverPacket()) {
                        ptr->dispatch_packet(entt::null, RoseCommon::fetchPacket<true>(static_cast<const uint8_t*>(P.get_blob().data())));
                    } else {
                        ptr->dispatch_packet(entt::null, RoseCommon::fetchPacket<false>(static_cast<const uint8_t*>(P.get_blob().data())));
                    }
                }
            }
        }
    }
    return true;
}

bool CMapISC::transfer_char(RoseCommon::Packet::IscTransferChar&& P) {
    for (const auto name : P.get_names()) {
        for (auto& [_, map] : maps) {
            if (auto ptr = map.lock()) {
                auto entity = ptr->get_entity_from_name(name);
                if (entity != entt::null) {
                    if (P.get_serverPacket()) {
                        ptr->dispatch_packet(entity, RoseCommon::fetchPacket<true>(static_cast<const uint8_t*>(P.get_blob().data())));
                    } else {
                        ptr->dispatch_packet(entity, RoseCommon::fetchPacket<false>(static_cast<const uint8_t*>(P.get_blob().data())));
                    }
                }
            }
        }
    }
    return true;
}

bool CMapISC::isChar() const { return socket_[SocketType::Client]->get_type() == Isc::ServerType::CHAR; }

bool CMapISC::handlePacket(uint8_t* _buffer) {
  switch (CRosePacket::type(_buffer)) {
    case ePacketType::ISC_ALIVE:
      return true;
    case ePacketType::PAKCS_LOGIN_REQ:
      return true;
    case ePacketType::ISC_SERVER_REGISTER:
      return serverRegister(Packet::IscServerRegister::create(_buffer));
    case ePacketType::ISC_TRANSFER:
      return transfer(Packet::IscTransfer::create(_buffer));
    case ePacketType::ISC_TRANSFER_CHAR:
      return transfer_char(Packet::IscTransferChar::create(_buffer));
    case ePacketType::ISC_SHUTDOWN:
      return true;
    default: {
      CRoseISC::handlePacket(_buffer);
      return false;
    }
  }
  return true;
}

bool CMapISC::serverRegister(RoseCommon::Packet::IscServerRegister&& P) {
  logger_->trace("CMapISC::serverRegister");

  //  int16_t _type = 0;
  //  _type = pMapServer.type();

  // 1 == char server
  // 2 == node server
  // 3 == map master server (This is the only type the login server will care
  // about)
  // 4 == map workers/threads

  //  iscPacket::ServerReg pServerReg;
  //  std::string name, ip;
  //  int32_t port = 0, type = 0, right = 0;

  //  if (_type == iscPacket::ServerType::NODE) {
  //    // This is a node and we need to figure out something to do with this
  //  } else if (_type == iscPacket::ServerType::MAP_MASTER) {
  //    name = pMapServer.name();
  //    ip = pMapServer.addr();
  //    port = pMapServer.port();
  //    type = pMapServer.type();
  //    right = pMapServer.accright();
  //  } else if (_type == iscPacket::ServerType::MAP_WORKER) {
  //    name = pMapServer.name();
  //    ip = pMapServer.addr();
  //    port = pMapServer.port();
  //    type = pMapServer.type();
  //    right = pMapServer.accright();
  //    this->set_type(_type);
  //  }

  logger_->info("ISC Server {} Connected: [{}, {}, {}:{}]\n", get_id(), RoseCommon::Isc::serverTypeName(P.get_serverType()), P.get_name(),
                P.get_addr(), P.get_port());
  return false;
}

void CMapISC::onConnected() {
  Core::Config& config = Core::Config::getInstance();
  {
    auto packet = Packet::CliLoginReq::create(
        config.mapServer().charPassword,
        config.mapServer().charUser);

    logger_->trace("Sending a packet on CMapISC: Header[{0}, 0x{1:x}]",
                   packet.get_size(), (uint16_t)packet.get_type());
    send(packet);
  }
  {
    auto packet = Packet::IscServerRegister::create(
        RoseCommon::Isc::ServerType::MAP_MASTER, config.mapServer().channelName, config.serverData().externalIp,
        config.mapServer().clientPort, config.mapServer().accessLevel, get_id());

    std::vector<uint16_t> m;
    m.reserve(maps.size());
    for (const auto& it : maps) {
        m.push_back(it.first);
    }
    packet.set_maps(m);

    logger_->trace("Sending a packet on CMapISC: Header[{0}, 0x{1:x}]", packet.get_size(),
                   static_cast<uint16_t>(packet.get_type()));
    send(packet);
  }

  if (socket_[SocketType::Client]->process_thread_.joinable() == false) {
    socket_[SocketType::Client]->process_thread_ = std::thread([this]() {
      while (is_active() == true && isChar() == true) {
        std::chrono::steady_clock::time_point update = Core::Time::GetTickCount();
        int64_t dt = std::chrono::duration_cast<std::chrono::milliseconds>(update - get_update_time()).count();
        if (dt > std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::minutes(4)).count())
        {
          logger_->trace("Sending ISC_ALIVE");
          send(Packet::IscAlive::create());
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      }
      return 0;
    });
  }
}

bool CMapISC::onShutdown() {
  logger_->trace("CMapISC::onDisconnected");
  bool result = true;

  if (is_active() == true) {
    if (get_type() == RoseCommon::Isc::ServerType::CHAR) {
      if (socket_[SocketType::Client]->reconnect() == true) {
        logger_->info("Reconnected to character server.");
        result = false;
      }
    } else {
      auto packet = Packet::IscShutdown::create(get_type(), get_id());
      std::lock_guard<std::mutex> lock(server_->GetISCListMutex());
      for (auto& server : server_->GetISCList()) {
        CMapISC* svr = static_cast<CMapISC*>(server.get());
        if (!svr) {
          continue;
        }
        if (svr->get_type() == RoseCommon::Isc::ServerType::CHAR) {
          svr->send(packet);
          break;
        }
      }
    }
  }
  return result;
}

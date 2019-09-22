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

#include "ccharserver.h"
#include "ccharclient.h"
#include "ccharisc.h"
#include "epackettype.h"
#include "platform_defines.h"
#include "connection.h"
#include <unordered_set>
#include "isc_client_status.h"
#include "logconsole.h"

using namespace RoseCommon;

void update_status(const Packet::IscClientStatus& packet, CCharServer& server) {
    auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
    if (auto user = server.get_user(packet.get_charId()); user) {
        logger->debug("Char {} now has status {}", packet.get_charId(), packet.get_status());
        const bool isSwitching = user.value()->get_status() == User::Status::SWITCHING ? true : false;
        user.value()->set_status(packet.get_status());
        if (user.value()->get_status() == User::Status::CONNECTED && isSwitching) {
            // reload the map
            Core::CharacterTable characterTable{};
            auto conn = Core::connectionPool.getConnection<Core::Osirose>();

            auto charRes = conn(sqlpp::select(characterTable.map)
                                  .from(characterTable).where(characterTable.id == user.value()->get_charId()));

            if (charRes.empty()) {
                logger->error("Error while trying to access the updated map of {}", user.value()->get_charId());
                return;
            }
            user.value()->set_mapId(charRes.front().map);
        }
    } else {
        logger->error("Error, got status packet for un-loaded {} client", packet.get_charId());
    }
}

CCharServer::CCharServer(bool _isc, CCharServer *server) : CRoseServer(_isc), client_count_(0), server_count_(0), iscServer_(server) {
    register_dispatcher(std::function{update_status});

    reactor_thread = std::thread([this]() {
        for (auto [res, task] = work_queue.pop_front(); res;) {
            {
                std::lock_guard<std::recursive_mutex> lock(access);
                std::invoke(std::move(task), *this);
            }
            auto [tmp_res, tmp_task] = work_queue.pop_front();
            res = tmp_res;
            task = std::move(tmp_task);
        }
    });
}

CCharServer::~CCharServer() {
    socket_[SocketType::Client]->shutdown();
    work_queue.kill();
    reactor_thread.join();
}

void CCharServer::OnAccepted(std::unique_ptr<Core::INetwork> _sock) {
  //if (_sock->is_active()) {
    // Do Something?
    std::string _address = _sock->get_address();
    if (IsISCServer() == false) {
      std::lock_guard<std::mutex> lock(client_list_mutex_);
      std::shared_ptr<CCharClient> nClient = std::make_shared<CCharClient>(this, std::move(_sock));
      nClient->set_id(client_count_++);
      nClient->set_update_time( Core::Time::GetTickCount() );
      nClient->set_active(true);
      nClient->start_recv();
      logger_->info( "[{}] Client connected from: {}", nClient->get_id(),
                       _address.c_str());
      client_list_.push_front(std::move(nClient));
    } else {
      std::lock_guard<std::mutex> lock(isc_list_mutex_);
      std::shared_ptr<CCharISC> nClient = std::make_shared<CCharISC>(this, std::move(_sock));
      nClient->set_id(server_count_++);
      nClient->set_update_time( Core::Time::GetTickCount() );
      nClient->set_active(true);
      nClient->start_recv();
      logger_->info( "Server connected from: {}", _address.c_str() );
      isc_list_.push_front(std::move(nClient));
    }
  //}
}

void CCharServer::register_maps(CCharISC* isc, const std::vector<uint16_t>& maps) {
    std::shared_ptr<RoseCommon::CRoseClient> ptr;
    for (const auto& p : isc_list_) {
        if (p.get() == isc) {
            ptr = p;
            break;
        }
    }
    if (!ptr) {
        logger_->error("ISC server not found when registering maps!");
        return;
    }
    for (const auto& m : maps) {
        this->maps[m] = ptr;
    }
}

void CCharServer::transfer(RoseCommon::Packet::IscTransfer&& P) {
    const auto& m = P.get_maps();
    std::unordered_set<std::shared_ptr<CRoseClient>> set;
    if (m.empty()) {
        for (const auto& [m, p] : maps) {
            if (auto ptr = p.lock()) {
                set.insert(ptr);
            }
        }
    } else if (m.size() == 1 && m[0] == 0) {
        dispatch_packet(RoseCommon::fetchPacket<true>(static_cast<const uint8_t*>(P.get_blob().data())));
        return;
    } else {
        for (const auto& mm : m) {
            if (auto ptr = maps[mm].lock()) {
                set.insert(ptr);
            }
        }
    }
    for (auto ptr : set) {
        ptr->send(P);
    }
}

void CCharServer::transfer_char(RoseCommon::Packet::IscTransferChar&& P) {
    std::vector<uint16_t> maps;
    for (auto name : P.get_names()) {
        if (const auto user = get_user(name); user) {
            maps.push_back(user.value()->get_mapId());
        }
    }
    std::unordered_set<std::shared_ptr<CRoseClient>> set;
    for (auto map : maps) {
        if (auto ptr = this->maps[map].lock()) {
            set.insert(ptr);
        }
    }
    for (auto ptr : set) {
        ptr->send(P);
    }
}

void CCharServer::send_map(uint16_t map, const RoseCommon::CRosePacket& p) {
    auto packet = RoseCommon::Packet::IscTransfer::create({map});
    std::vector<uint8_t> blob;
    p.write_to_vector(blob);
    packet.set_blob(blob);
    if (auto ptr = maps[map].lock()) {
        ptr->send(packet);
    }
}

void CCharServer::send_char(uint32_t character, const RoseCommon::CRosePacket& p) {
    const auto user = get_user(character);
    if (!user) {
        return;
    }
    auto packet = RoseCommon::Packet::IscTransferChar::create({user.value()->get_name()});
    std::vector<uint8_t> blob;
    p.write_to_vector(blob);
    packet.set_blob(blob);

    if (auto ptr = maps[user.value()->get_mapId()].lock()) {
        ptr->send(packet);
    }
}

void CCharServer::send_char(const std::string& character, const RoseCommon::CRosePacket& p) {
    const auto user = get_user(character);
    if (!user) {
        return;
    }
    auto packet = RoseCommon::Packet::IscTransferChar::create({character});
    std::vector<uint8_t> blob;
    p.write_to_vector(blob);
    packet.set_blob(blob);

    if (auto ptr = maps[user.value()->get_mapId()].lock()) {
        ptr->send(packet);
    }
}

bool CCharServer::dispatch_packet(std::unique_ptr<RoseCommon::CRosePacket>&& packet) {
    if (!packet) {
        return false;
    }
    if (!dispatcher.is_supported(*packet.get())) {
        return false;
    }
    work_queue.push_back([packet = std::move(packet)](CCharServer& server) mutable {
        server.dispatcher.dispatch(std::move(packet), server);
    });
    return true;
}

std::optional<const User*const> CCharServer::get_user(const std::string& name) const {
    for (auto [k, v] : users) {
        if (v.get_name() == name) {
            return {&v};
        }
    }
    return {};
}

std::optional<const User*const> CCharServer::get_user(uint32_t id) const {
    if (auto it = users.find(id); it != users.end()) {
        return {&it->second};
    }
    return {};
}

std::optional<User*const> CCharServer::get_user(const std::string& name) {
    for (auto [k, v] : users) {
        if (v.get_name() == name) {
            return {&v};
        }
    }
    return {};
}

std::optional<User*const> CCharServer::get_user(uint32_t id) {
    if (auto it = users.find(id); it != users.end()) {
        return {&it->second};
    }
    return {};
}

void CCharServer::load_user(uint32_t id) {
    Core::CharacterTable characterTable{};

    auto conn = Core::connectionPool.getConnection<Core::Osirose>();

    auto charRes = conn(sqlpp::select(characterTable.name, characterTable.map)
                          .from(characterTable).where(characterTable.id == id));

    if (charRes.empty()) {
        return;
    }
    User user(charRes.front().name, id, charRes.front().map);
    user.set_party(partys.get_party(id)); // we load the party if there is one for that character
}

void CCharServer::unload_user(uint32_t id) {
    users.erase(id);
}

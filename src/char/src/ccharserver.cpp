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

#include "srv_party_member.h"

#include <algorithm>

using namespace RoseCommon;

void update_status(const Packet::IscClientStatus& packet, CCharServer& server, User& user) {
    auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
    static const char* status[] = {
        "CONNECTED",
        "DISCONNECTED",
        "SWITCHING",
        "AFK",
        "INVISIBLE"
    };
    logger->debug("Char {} now has status {}", user.get_name(), status[packet.get_status()]);
    const bool isSwitching = user.get_status() == User::Status::SWITCHING ? true : false;
    user.set_status(packet.get_status());
    // we update the id every client on the map refers to when talking about this character. (this is different from the charId)
    user.set_entityId(packet.get_entityMapId());
    if (user.get_status() == User::Status::CONNECTED && isSwitching) {
        // reload the map
        Core::CharacterTable characterTable{};
        auto conn = Core::connectionPool.getConnection<Core::Osirose>();
        auto charRes = conn(sqlpp::select(characterTable.map)
                            .from(characterTable).where(characterTable.id == user.get_charId()));
        if (charRes.empty()) {
            logger->error("Error while trying to access the updated map of {}", user.get_name());
            return;
        }
        user.set_mapId(charRes.front().map);
    } else if (user.get_status() == User::Status::DISCONNECTED) {
        server.unload_user(user.get_charId());
    }
}

CCharServer::CCharServer(bool _isc, CCharServer *server) : CRoseServer(_isc), client_count_(0), server_count_(0), iscServer_(server) {
    register_dispatcher(std::function{update_status});
    register_dispatcher(std::function{party_request});
    register_dispatcher(std::function{party_reply});

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
        if (P.get_serverPacket()) {
            dispatch_packet(P.get_originatorId(),
                            RoseCommon::fetchPacket<true>(static_cast<const uint8_t*>(P.get_blob().data())));
        } else {
            dispatch_packet(P.get_originatorId(),
                            RoseCommon::fetchPacket<false>(static_cast<const uint8_t*>(P.get_blob().data())));
        }
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
    auto packet = RoseCommon::Packet::IscTransfer::create(0, {map});
    packet.set_serverPacket(p.get_server_packet());
    std::vector<uint8_t> blob;
    p.write_to_vector(blob);
    packet.set_blob(blob);
    if (auto ptr = maps[map].lock()) {
        ptr->send(packet);
    }
}

void CCharServer::send_char(uint32_t character, RoseCommon::CRosePacket&& packet) {
    const auto user = get_user(character);
    if (!user) {
        logger_->error("User {} not found!", character);
        return;
    }
    send_char(*user.value(), std::move(packet));
}

void CCharServer::send_char(const User& user, RoseCommon::CRosePacket&& p) {
    /*if (auto client = user.get_client().lock(); client) {
        client->send_packet(packet);
    }*/
    auto packet = RoseCommon::Packet::IscTransferChar::create({user.get_name()});
    packet.set_serverPacket(p.get_server_packet());
    std::vector<uint8_t> blob;
    p.write_to_vector(blob);
    packet.set_blob(blob);
    if (auto ptr = maps[user.get_mapId()].lock()) {
        ptr->send(packet);
    }
}

void CCharServer::send_char(const std::string& character, RoseCommon::CRosePacket&& packet) {
    const auto user = get_user(character);
    if (!user) {
        logger_->error("User {} not found!", character);
        return;
    }
    send_char(*user.value(), std::move(packet));
}

bool CCharServer::dispatch_packet(uint32_t charId, std::unique_ptr<RoseCommon::CRosePacket>&& packet) {
    if (!packet) {
        logger_->error("Empty/not parsable packet");
        return false;
    }
    if (!dispatcher.is_supported(*packet.get())) {
        logger_->error("Packet not supported!");
        return false;
    }
    if (!get_user(charId)) {
        logger_->error("User {} not loaded!", charId);
        return false;
    }
    work_queue.push_back([charId, packet = std::move(packet)](CCharServer& server) mutable {
        auto user = server.get_user(charId);
        if (!user) {
            server.logger_->warn("cancelling action on disconnected user");
            return;
        }
        server.dispatcher.dispatch(std::move(packet), server, std::forward<User&>(*user.value()));
    });
    return true;
}

std::optional<const User*const> CCharServer::get_user(const std::string& name) const {
    if (const auto result = std::find_if(users.begin(), users.end(), 
                                         [&name](const auto& user) { return user.second.get_name() == name; });
        result != users.end()) {
        return {&result->second};
    }
    return {};
}

std::optional<const User*const> CCharServer::get_user(uint32_t id) const {
    if (auto it = users.find(id); it != users.end()) {
        return {&it->second};
    }
    return {};
}

std::optional<const User*const> CCharServer::get_user(uint16_t id, uint16_t mapId) const {
    if (const auto result = std::find_if(users.begin(), users.end(), [id, mapId](const auto& user) {
            return user.second.get_mapId() == mapId && user.second.get_entityId() == id;
        }); result != users.end()) {
        return {&result->second};
    }
    return {};
}

std::optional<User*const> CCharServer::get_user(const std::string& name) {
    if (auto result = std::find_if(users.begin(), users.end(), 
                                   [&name](const auto& user) { return user.second.get_name() == name; });
        result != users.end()) {
        return {&result->second};
    }
    return {};
}

std::optional<User*const> CCharServer::get_user(uint32_t id) {
    if (auto it = users.find(id); it != users.end()) {
        return {&it->second};
    }
    return {};
}
    
std::optional<User*const> CCharServer::get_user(uint16_t id, uint16_t mapId) {
    if (auto result = std::find_if(users.begin(), users.end(), [id, mapId](const auto& user) {
            return user.second.get_mapId() == mapId && user.second.get_entityId() == id;
        }); result != users.end()) {
        return {&result->second};
    }
    return {};
}

void CCharServer::load_user(std::weak_ptr<CCharClient> client, uint32_t id) {
    if (IsISCServer() == false) {
        iscServer_->load_user(client, id);
        return;
    }
    Core::CharacterTable characterTable{};

    auto conn = Core::connectionPool.getConnection<Core::Osirose>();

    auto charRes = conn(sqlpp::select(characterTable.name, characterTable.map)
                          .from(characterTable).where(characterTable.id == id));

    if (charRes.empty()) {
        return;
    }
    User user(client, charRes.front().name, id, charRes.front().map);
    user.set_party(partys.get_party(id)); // we load the party if there is one for that character

    users.emplace(std::make_pair(id, user));
}

void CCharServer::unload_user(uint32_t id) {
    if (IsISCServer() == false) {
        iscServer_->unload_user(id);
    } else {
        if (const auto user = get_user(id); user) {
            if (user.value()->get_party()) {
                for (const auto& member : user.value()->get_party()->members) {
                    if (users.count(member) && users.at(member).get_requested_party()) {
                        users.at(member).set_requested_party({});
                    }
                }
                partys.remove_member_from_party(user.value()->get_party(), user.value()->get_charId());
            } else if (user.value()->get_requested_party()) {
                partys.remove_party(user.value()->get_requested_party());
            }
        }
        users.erase(id);
    }
}

std::shared_ptr<Party> CCharServer::create_party(User& user) {
    auto party = partys.create_party(user.get_charId());
    user.set_party(party);
    return party;
}

void CCharServer::add_user_to_party(User& user, std::shared_ptr<Party> party) {
    user.set_party(party);

    RoseCommon::Packet::SrvPartyMember::PartyMember member;

    member.set_tag(user.get_charId());
    member.set_id(user.get_mapId());
    member.set_max_hp(1);
    member.set_hp(1);
    member.set_status(0);
    member.set_con(1);
    member.set_hp_recovery(1);
    member.set_mp_recovery(1);
    member.set_stamina(1);
    member.set_name(user.get_name());

    for (const auto& user : party->members) {
        auto packet = RoseCommon::Packet::SrvPartyMember::create(
                static_cast<RoseCommon::Packet::SrvPartyMember::PartyRule>(party->options),
                RoseCommon::Packet::SrvPartyMember::ADD, member);
        send_char(user, std::move(packet));
    }

    partys.add_member_to_party(party, user.get_charId());
}

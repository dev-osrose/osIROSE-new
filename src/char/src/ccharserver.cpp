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

using namespace RoseCommon;

CCharServer::CCharServer(bool _isc, CCharServer *server) : CRoseServer(_isc), client_count_(0), server_count_(0), iscServer_(server) {
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
    Core::SessionTable sessions{};
    Core::CharacterTable characters{};

    auto conn = Core::connectionPool.getConnection<Core::Osirose>();

    std::vector<uint16_t> maps;
    for (auto name : P.get_names()) {
        const auto res = conn(
                sqlpp::select(characters.name, characters.map).from(characters.join(sessions).on(sessions.charid == characters.id))
                    .where(characters.name == name)
            );
        if (res.empty()) {
            continue;
        }
        maps.push_back(res.front().map);
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
    Core::CharacterTable characters{};

    auto conn = Core::connectionPool.getConnection<Core::Osirose>();

    const auto res = conn(
            sqlpp::select(characters.name, characters.map).from(characters).where(characters.id == character)
        );
    if (res.empty()) {
        return; // logged out
    }

    auto packet = RoseCommon::Packet::IscTransferChar::create({res.front().name});
    std::vector<uint8_t> blob;
    p.write_to_vector(blob);
    packet.set_blob(blob);

    if (auto ptr = maps[res.front().map].lock()) {
        ptr->send(packet);
    }
}

void CCharServer::send_char(const std::string& character, const RoseCommon::CRosePacket& p) {
    Core::SessionTable sessions{};
    Core::CharacterTable characters{};

    auto conn = Core::connectionPool.getConnection<Core::Osirose>();

    const auto res = conn(
            sqlpp::select(characters.name, characters.map).from(characters.join(sessions).on(sessions.charid == characters.id))
                .where(characters.name == character)
        );
    if (res.empty()) {
        return; // logged out
    }

    auto packet = RoseCommon::Packet::IscTransferChar::create({character});
    std::vector<uint8_t> blob;
    p.write_to_vector(blob);
    packet.set_blob(blob);

    if (auto ptr = maps[res.front().map].lock()) {
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

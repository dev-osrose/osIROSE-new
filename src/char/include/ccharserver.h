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

#ifndef __CCHARSERVER_H__
#define __CCHARSERVER_H__

#include "croseserver.h"
#include <vector>
#include <unordered_map>
#include <deque>
#include <string_view>
#include <mutex>
#include <thread>
#include <optional>
#include "isc_transfer.h"
#include "isc_transfer_char.h"
#include "mwsrqueue.h"
#include "fire_once.h"
#include "crosepacket.h"
#include "packet_dispatcher.h"
#include "user.h"
#include "party.h"

class CCharISC;

class CCharServer : public RoseCommon::CRoseServer {
 public:
  CCharServer(bool _isc = false, CCharServer *iscServer = nullptr);
  virtual ~CCharServer();

  std::forward_list<std::shared_ptr<RoseCommon::CRoseClient>>& GetISCList() {
      if (iscServer_)
          return iscServer_->GetISCList();
      return RoseCommon::CRoseServer::GetISCList();
  }
  std::mutex& GetISCListMutex() {
      if (iscServer_)
          return iscServer_->GetISCListMutex();
      return RoseCommon::CRoseServer::GetISCListMutex();
  }

  void register_maps(CCharISC*, const std::vector<uint16_t>&);
  void transfer(RoseCommon::Packet::IscTransfer&& P);
  void transfer_char(RoseCommon::Packet::IscTransferChar&& P);
  void send_map(uint16_t map, const RoseCommon::CRosePacket& packet);
  void send_char(uint32_t character, RoseCommon::CRosePacket&& packet);
  void send_char(const User& user, RoseCommon::CRosePacket&& packet);
  void send_char(const std::string& character, RoseCommon::CRosePacket&& packet);

  std::optional<const User*const> get_user(const std::string& name) const;
  std::optional<const User*const> get_user(uint32_t charId) const;
  std::optional<const User*const> get_user(uint16_t id, uint16_t mapId) const;
  std::optional<User*const> get_user(const std::string& name);
  std::optional<User*const> get_user(uint32_t charId);
  std::optional<User*const> get_user(uint16_t id, uint16_t mapId);
  void load_user(std::weak_ptr<CCharClient> socket, uint32_t id);
  void unload_user(uint32_t id);
 
  std::shared_ptr<Party> create_party(User& user);
  void add_user_to_party(User& user, std::shared_ptr<Party> party);

 protected:
  virtual void OnAccepted(std::unique_ptr<Core::INetwork> _sock) override;
  uint32_t client_count_;
  uint32_t server_count_;

  bool dispatch_packet(uint32_t charId, std::unique_ptr<RoseCommon::CRosePacket>&& packet);

 private:
  CCharServer *iscServer_;
  std::unordered_map<uint16_t, std::weak_ptr<RoseCommon::CRoseClient>> maps;
  RoseCommon::PacketDispatcher<CCharServer&, User&> dispatcher;
  Core::MWSRQueue<std::deque<Core::fire_once<void(CCharServer&)>>> work_queue;
  std::thread reactor_thread;
  std::recursive_mutex access;

  std::unordered_map<uint32_t, User> users;
  PartyManager partys;

  template <typename T>
  void register_dispatcher(std::function<void(const T&, CCharServer&, User&)>&& func) {
      dispatcher.add_dispatcher(T::PACKET_ID, std::move(func));
  }
};

#endif

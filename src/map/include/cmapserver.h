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

#ifndef _CMAPSERVER_H_
#define _CMAPSERVER_H_

#include <optional>

#include "croseserver.h"
#include "crosepacket.h"
#include <chrono>

#include "entity_system.h"

class CMapISC;

class CMapServer : public RoseCommon::CRoseServer {
 public:
  CMapServer(bool _isc = false, int16_t mapidx = -1, CMapServer* iscServer = nullptr, CMapISC* isc_client_ = nullptr);
  virtual ~CMapServer();

  int16_t GetMapIDX() const { return map_idx_; }
  std::shared_ptr<EntitySystem> get_entity_system() { return entitySystem; }

  void stop();
  void run();

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

  void send_to_maps(const RoseCommon::CRosePacket& p, const std::vector<uint16_t>& maps, uint32_t originatorId);
  void send_to_chars(const RoseCommon::CRosePacket& p, const std::vector<std::string>& chars);

 protected:
  virtual void OnAccepted(std::unique_ptr<Core::INetwork> _sock);

  enum class ServerType : int8_t { MASTER_NODE = -1, WORKER_THREAD };
  int16_t map_idx_;
  uint32_t client_count_;
  uint32_t server_count_;

 private:
  CMapServer *iscServer_;
  CMapISC    *isc_client_;
  std::shared_ptr<EntitySystem> entitySystem;
};

#endif

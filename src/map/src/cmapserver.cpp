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

#include "config.h"
#include "cmapserver.h"
#include "cmapclient.h"
#include "cmapisc.h"
#include "epackettype.h"
#include "platform_defines.h"
#include "config.h"
#include "isc_transfer.h"
#include "isc_transfer_char.h"

using namespace RoseCommon;

CMapServer::CMapServer(bool _isc, int16_t mapidx, CMapServer *server, CMapISC *client)
    : CRoseServer(_isc),
      map_idx_(mapidx),
      client_count_(0),
      server_count_(0),
      iscServer_(server),
      isc_client_(client) {
  if (mapidx >= 0) {
    // We are a worker thread/process
    // We need to connect to the master thread/process to get data to handle
    entitySystem = std::make_shared<EntitySystem>(map_idx_, this);
  } else {
    // We are a master/node process
    // We accept player connections and redirect their packet data to the
    // correct worker
  }
}

CMapServer::~CMapServer() {}

void CMapServer::OnAccepted(std::unique_ptr<Core::INetwork> _sock) {
  // if (_sock->is_active()) {
  // Do Something?
  std::string _address = _sock->get_address();
  if (IsISCServer() == false) {
    std::lock_guard<std::mutex> lock(client_list_mutex_);
    std::shared_ptr<CMapClient> nClient = std::make_shared<CMapClient>(std::move(_sock), entitySystem);
    nClient->set_id(++client_count_);
    nClient->set_update_time(Core::Time::GetTickCount());
    nClient->set_active(true);
    nClient->start_recv();
    logger_->info("Client connected from: {}", _address.c_str());
    client_list_.push_front(std::move(nClient));
  } else {
    std::lock_guard<std::mutex> lock(isc_list_mutex_);
    std::shared_ptr<CMapISC> nClient = std::make_shared<CMapISC>(this, std::move(_sock));
    nClient->set_id(server_count_++);
    nClient->set_update_time(Core::Time::GetTickCount());
    nClient->set_active(true);
    nClient->start_recv();
    logger_->info("Server connected from: {}", _address.c_str());
    isc_list_.push_front(std::move(nClient));
  }
  //}
}

void CMapServer::stop() { entitySystem->stop(); }
void CMapServer::run() { entitySystem->run(); }

void CMapServer::send_to_maps(const RoseCommon::CRosePacket& p, const std::vector<uint16_t>& maps, uint32_t originatorId) {
    auto packet = RoseCommon::Packet::IscTransfer::create(originatorId, maps);
    packet.set_serverPacket(p.get_server_packet());
    std::vector<uint8_t> blob;
    p.write_to_vector(blob);
    packet.set_blob(blob);
    isc_client_->send(packet);
}

void CMapServer::send_to_chars(const RoseCommon::CRosePacket& p, const std::vector<std::string>& chars) {
    auto packet = RoseCommon::Packet::IscTransferChar::create(chars);
    packet.set_serverPacket(p.get_server_packet());
    std::vector<uint8_t> blob;
    p.write_to_vector(blob);
    packet.set_blob(blob);
    isc_client_->send(packet);
}

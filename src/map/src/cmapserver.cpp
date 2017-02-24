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

#include <sol.hpp>
#include "cmapserver.h"
#include "cmapclient.h"
#include "cmapisc.h"
#include "epackettype.h"

using namespace RoseCommon;

CMapServer::CMapServer(bool _isc, int16_t mapidx)
    : CRoseServer(_isc), map_idx_(mapidx), client_count_(0), server_count_(0), entitySystem_(std::make_shared<EntitySystem>()) {

	sol::state lua;
	lua.open_libraries(sol::lib::base);
	lua.set_function("log", [this](std::string str) { logger_->info(str.c_str()); });
	//lua.script_file("./scripts/main.lua");

  if (mapidx >= 0) {
    // We are a worker thread/process
    // We need to connect to the master thread/process to get data to handle
  } else {
    // We are a master/node process
    // We accept player connections and redirect their packet data to the
    // correct worker
  }
}

void CMapServer::SendPacket(const CMapClient* sender, CMapServer::eSendType type, CRosePacket &_buffer) {
    CRoseServer::SendPacket(dynamic_cast<const CRoseClient*>(sender), type, _buffer);
}

CMapServer::~CMapServer() {}

void CMapServer::OnAccepted(int* _sock) {
//  if (_sock.is_open()) {
    // Do Something?
//    std::string _address = _sock.remote_endpoint().address().to_string();
    if (IsISCServer() == false) {
      std::lock_guard<std::mutex> lock(client_list_mutex_);
      CMapClient* nClient = new CMapClient(std::move(_sock), entitySystem_);
      nClient->SetLastUpdateTime(Core::Time::GetTickCount());
      nClient->SetId(++client_count_);
//      logger_->info("Client connected from: {}", _address.c_str());
      client_list_.push_front(nClient);
    } else {
      std::lock_guard<std::mutex> lock(isc_list_mutex_);
      CMapISC* nClient = new CMapISC(std::move(_sock));
      nClient->SetLastUpdateTime(Core::Time::GetTickCount());
      nClient->SetId( server_count_++ );
//      logger_->info( "Server connected from: {}", _address.c_str() );
      isc_list_.push_front(nClient);
    }
//  }
}

void CMapServer::update(double dt) {
    entitySystem_->update(dt);
}

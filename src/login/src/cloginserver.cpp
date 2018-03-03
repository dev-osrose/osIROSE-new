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

#include "cloginserver.h"
#include "cloginclient.h"
#include "cloginisc.h"
#include "epackettype.h"
#include "config.h"
#include "platform_defines.h"

CLoginServer::CLoginServer(bool _isc) : CRoseServer(_isc), client_count_(0), server_count_(0) {
}

CLoginServer::~CLoginServer() { socket_->shutdown(true); }

void CLoginServer::OnAccepted(std::unique_ptr<Core::INetwork> _sock) {
  //if (is_active()) {
    // Do Something?
    std::string _address = _sock->get_address();
    if (IsISCServer() == false) {
      std::lock_guard<std::mutex> lock(client_list_mutex_);
      std::shared_ptr<CLoginClient> nClient = std::make_shared<CLoginClient>(std::move(_sock));
      nClient->set_id(client_count_++);
      nClient->set_update_time( Core::Time::GetTickCount() );
      nClient->set_active(true);
      nClient->start_recv();
      logger_->info( "[{}] Client connected from: {}", nClient->get_id(),
              _address.c_str());
      client_list_.push_front(std::move(nClient));
    } else {
      std::lock_guard<std::mutex> lock(isc_list_mutex_);
      std::shared_ptr<CLoginISC> nClient = std::make_shared<CLoginISC>(std::move(_sock));
      nClient->set_id(server_count_++);
      nClient->set_update_time(Core::Time::GetTickCount());
      nClient->set_active(true);
      nClient->start_recv();
      logger_->info("Server connected from: {}", _address.c_str());
      isc_list_.push_front(std::move(nClient));
    }
  //}
}

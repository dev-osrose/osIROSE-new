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

#include "cnetwork_asio.h"
#include "nodeserver.h"
#include "nodeclient.h"
#include "nodeisc.h"
#include "epackettype.h"
#include "config.h"
#include "platform_defines.h"

NodeServer::NodeServer(bool _isc) : CRoseServer(_isc), client_count_(0), server_count_(0) {
}

NodeServer::~NodeServer() { socket_[RoseCommon::SocketType::Client]->shutdown(true); }

void NodeServer::OnAccepted(std::unique_ptr<Core::INetwork> _sock) {
  std::string _address = _sock->get_address();

  std::lock_guard<std::mutex> lock(isc_list_mutex_);
  std::shared_ptr<NodeClient> nClient = std::make_shared<NodeClient>(std::move(_sock));
  nClient->set_id(server_count_++);
  nClient->set_update_time(Core::Time::GetTickCount());
  nClient->set_active(true);
  nClient->start_recv();
  
  nClient->set_socket(std::make_unique<Core::CNetwork_Asio>(), RoseCommon::SocketType::CurrentMap, true);
  
  // Core::Config& config = Core::Config::getInstance();
  
  // // Disconnect from the current server
  // disconnect(RoseCommon::SocketType::CurrentMap);
  
  // // Set up our socket to connect to the login server
  // init(config.nodeServer().loginIp, config.nodeServer().loginPort, RoseCommon::SocketType::CurrentMap);
  
  // // Actually connect to the new server
  // connect(RoseCommon::SocketType::CurrentMap);
  
  logger_->info("Client connected from: {}", _address.c_str());
  client_list_.push_front(std::move(nClient));
}

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

#include <stdint.h>
#include "logconsole.h"
#include "croseserver.h"
#include "platform_defines.h"
#include "croseisc.h"

#define USE_ASIO_NETWORKING
#ifdef USE_ASIO_NETWORKING
#include "cnetwork_asio.h"
#endif

namespace RoseCommon {

CRoseServer::CRoseServer(bool _iscServer) : CRoseSocket(std::make_unique<Core::CNetwork_Asio>()),
  isc_server_(_iscServer) {

  std::function<void(std::unique_ptr<Core::INetwork>)> fnOnAccepted = std::bind(&CRoseServer::OnAccepted, this, std::placeholders::_1);

  socket_[0]->registerOnAccepted(fnOnAccepted);

  socket_[0]->process_thread_ = std::thread([this]() {
    while(is_active() == false) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::forward_list<std::shared_ptr<CRoseClient>>* list_ptr = nullptr;
    std::mutex* mutex_ptr = nullptr;
    std::string inactive_log = "";
    std::string timeout_log = "";

    if (IsISCServer() == false) {
      list_ptr = &client_list_;
      mutex_ptr = &client_list_mutex_;
      inactive_log = "Client {} is inactive, removing the socket.";
      timeout_log = "Client {} timed out.";
    } else {
      list_ptr = &isc_list_;
      mutex_ptr = &isc_list_mutex_;
      inactive_log = "Server {} is inactive, removing the socket.";
      timeout_log = "Server {} timed out.";
    }

    do {
      (*mutex_ptr).lock();
        (*list_ptr).remove_if([this, inactive_log] (auto &i) {
            std::chrono::steady_clock::time_point update =
              Core::Time::GetTickCount();
            auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(
                           update - i->get_update_time());
            if (i->is_active() == false && dt > std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::seconds(30))  ) {
              logger_->debug(inactive_log.c_str(), i->get_id());
              return true;
            }
            return false;
          });

        for (auto& client : (*list_ptr)) {
          std::chrono::steady_clock::time_point update =
              Core::Time::GetTickCount();
          auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(
                           update - client->get_update_time());
          if (dt > std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::minutes(5)) && client->is_active() == true)  // wait some time before time out
          {
            logger_->info(timeout_log.c_str(), client->get_id());
            client->shutdown();
            // Do not delete them now. Do it next time.
          }
        }
      (*mutex_ptr).unlock();
      std::this_thread::sleep_for(std::chrono::milliseconds(250));
    } while (is_active() == true);

    if (logger_) {
        logger_->debug("CRoseServer::process_thread_::is_active was false, returning...");
    }
    return 0;
  });
}

CRoseServer::~CRoseServer() {
  if (is_active() == false) {
      set_active(true);
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      set_active(false);
  }
  if (IsISCServer() == false) {
    std::lock_guard<std::mutex> lock(client_list_mutex_);
    for (auto& client : client_list_) {
      client->shutdown(true);
    }
    client_list_.clear();
  } else {
    std::lock_guard<std::mutex> lock(isc_list_mutex_);
    for (auto& client : isc_list_) {
      client->shutdown(true);
    }
    isc_list_.clear();
  }
  for (auto& s : socket_) {
    if (s && s->process_thread_.joinable()) {
        s->process_thread_.join();
    }
  }
}

void CRoseServer::OnAccepted(std::unique_ptr<Core::INetwork> _sock) {

  logger_->warn(
      "CRoseServer::OnAccepted called! You should really overload this "
      "function.");
//  if (sock_.is_open()) {
    std::string _address = _sock->get_address();

    if (IsISCServer() == false) {
      std::lock_guard<std::mutex> lock(client_list_mutex_);
      auto nClient = std::make_shared<CRoseClient>(std::move(_sock));
      nClient->set_id(
          std::distance(std::begin(client_list_), std::end(client_list_)));
      nClient->start_recv();
      logger_->info("[{}] Client connected from: {}", nClient->get_id(),
        _address.c_str());
      client_list_.push_front(std::move(nClient));
    } else {
      std::lock_guard<std::mutex> lock(isc_list_mutex_);
      auto nClient = std::make_shared<CRoseISC>(std::move(_sock));
      nClient->set_id(std::distance(std::begin(isc_list_), std::end(isc_list_)));
      nClient->start_recv();
      logger_->info("[{}] Server connected from: {}", nClient->get_id(),
        _address.c_str());
      isc_list_.push_front(std::move(nClient));
    }
//  }
}
}

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

  socket_->registerOnAccepted(fnOnAccepted);

  socket_->process_thread_ = std::thread([this]() {

    std::forward_list<std::shared_ptr<CRoseClient>>* list_ptr = nullptr;
    std::mutex* mutex_ptr = nullptr;
    std::string inactive_log = "";
    std::string timeout_log = "";

    if (IsISCServer() == false) {
      list_ptr = &client_list_;
      mutex_ptr = &client_list_mutex_;
      inactive_log = "Client {} is inactive, shutting down the socket.";
      timeout_log = "Client {} timed out.";
    } else {
      list_ptr = &isc_list_;
      mutex_ptr = &isc_list_mutex_;
      inactive_log = "Server {} is inactive, shutting down the socket.";
      timeout_log = "Server {} timed out.";
    }

    do {
      (*mutex_ptr).lock();
        (*list_ptr).remove_if([this, inactive_log] (auto &i) {
            if (i->is_active() == false) {
              logger_->debug(inactive_log.c_str(), i->get_id());
              return true;
            }
            return false;
          });

        for (auto& client : (*list_ptr)) {
          std::chrono::steady_clock::time_point update =
              Core::Time::GetTickCount();
          int64_t dt = std::chrono::duration_cast<std::chrono::milliseconds>(
                           update - client->get_update_time())
                           .count();
          if (dt > (1000 * 60) * 2)  // wait 2 minutes before time out
          {
            logger_->info(timeout_log.c_str(), client->get_id());
            client->shutdown();
            // Do not delete them now. Do it next time.
          }
        }
      (*mutex_ptr).unlock();
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    } while (is_active() == true);

    return 0;
  });
}

CRoseServer::~CRoseServer() {
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
}

void CRoseServer::OnAccepted(std::unique_ptr<Core::INetwork> _sock) {

  logger_->warn(
      "CRoseServer::OnAccepted called! You should really overload this "
      "function.");
//  if (sock_.is_open()) {
    std::string _address = _sock->get_address();

    if (IsISCServer() == false) {
      std::lock_guard<std::mutex> lock(client_list_mutex_);
      auto nClient = std::make_unique<CRoseClient>(std::move(_sock));
      nClient->set_id(
          std::distance(std::begin(client_list_), std::end(client_list_)));
      nClient->start_recv();
      logger_->info("[{}] Client connected from: {}", nClient->get_id(),
        _address.c_str());
      client_list_.push_front(std::move(nClient));
    } else {
      std::lock_guard<std::mutex> lock(isc_list_mutex_);
      auto nClient = std::make_unique<CRoseISC>(std::move(_sock));
      nClient->set_id(std::distance(std::begin(isc_list_), std::end(isc_list_)));
      nClient->start_recv();
      logger_->info("[{}] Server connected from: {}", nClient->get_id(),
        _address.c_str());
      isc_list_.push_front(std::move(nClient));
    }
//  }
}

void CRoseServer::SendPacket(const CRoseClient* sender, eSendType type, CRosePacket &_buffer) {
  std::lock_guard<std::mutex> lock(client_list_mutex_);
  switch(type)
  {
    case eSendType::EVERYONE:
    {
      for (auto& client : client_list_) {
          client->send(_buffer);
      }
      break;
    }
    case eSendType::EVERYONE_BUT_ME:
    {
      for (auto& client : client_list_) {
        if( client.get() != sender)
          client->send(_buffer);
      }
      break;
    }
    case eSendType::EVERYONE_BUT_ME_ON_MAP:
        for (auto &client : client_list_)
            if (client.get() != sender && isOnMap(client->getEntity()))
                client->send(_buffer);
        break;
    case eSendType::NEARBY:
    {
      for (auto& client : client_list_) {
        if( client->is_nearby(sender) == true && isOnMap(client->getEntity()))
          client->send(_buffer);
      }
      break;
    }
    case eSendType::NEARBY_BUT_ME:
    {
      for (auto& client : client_list_) {
        if( client.get() != sender && client->is_nearby(sender) == true  && isOnMap(client->getEntity()))
          client->send(_buffer);
      }
      break;
    }
    default:
      break;
  }
}

}

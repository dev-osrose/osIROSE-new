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

namespace RoseCommon {

std::forward_list<CRoseClient*> CRoseServer::client_list_;
std::forward_list<CRoseISC*> CRoseServer::isc_list_;
std::mutex CRoseServer::client_list_mutex_;
std::mutex CRoseServer::isc_list_mutex_;

CRoseServer::CRoseServer(bool _iscServer) : isc_server_(_iscServer) {
  process_thread_ = std::thread([this]() {
    do {
      if (IsISCServer() == false) {
        std::lock_guard<std::mutex> lock(client_list_mutex_);
        client_list_.remove_if([this] (CRoseClient* i) {
            if (i->IsActive() == false) {
              logger_->debug("Client {} is inactive, shutting down the socket.", i->GetId());
              delete i;
              return true;
            }
            return false;
          });
        
        for (auto& client : client_list_) {
          std::chrono::steady_clock::time_point update =
              Core::Time::GetTickCount();
          int64_t dt = std::chrono::duration_cast<std::chrono::milliseconds>(
                           update - client->GetLastUpdateTime())
                           .count();
          if (dt > (1000 * 60) * 5)  // wait 5 minutes before time out
          {
            logger_->notice("Client {} timed out.", client->GetId());
            client->Shutdown();
            // Do not delete them now. Do it next time.
          }
        }
      } else {
        std::lock_guard<std::mutex> lock(isc_list_mutex_);
        isc_list_.remove_if([this] (CRoseISC* i) {
            if (i->IsActive() == false) {
              logger_->debug("Server {} is inactive, shutting down the socket.", i->GetId());
              delete i;
              return true;
            }
            return false;
          });
          
        for (auto& client : isc_list_) {
          std::chrono::steady_clock::time_point update =
              Core::Time::GetTickCount();
          int64_t dt = std::chrono::duration_cast<std::chrono::milliseconds>(
                           update - client->GetLastUpdateTime())
                           .count();
          if (dt > (1000 * 60) * 5)  // wait 5 minutes before time out
          {
            logger_->notice("Server {} timed out.", client->GetId());
            client->Shutdown();
            // Do not delete them now. Do it next time.
          }
        }
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    } while (active_ == true);

    return 0;
  });
}

CRoseServer::~CRoseServer() {
  Shutdown();
  process_thread_.join();

  if (IsISCServer() == false) {
    std::lock_guard<std::mutex> lock(client_list_mutex_);
    for (auto& client : client_list_) {
      client->Shutdown(true);
      delete client;
    }
    client_list_.clear();
  } else {
    std::lock_guard<std::mutex> lock(isc_list_mutex_);
    for (auto& client : isc_list_) {
      client->Shutdown(true);
      delete client;
    }
    isc_list_.clear();
  }
}

bool CRoseServer::OnConnect() { return true; }

void CRoseServer::OnConnected() {}

bool CRoseServer::OnListen() { return true; }

void CRoseServer::OnListening() {}

bool CRoseServer::OnDisconnect() { return true; }

void CRoseServer::OnDisconnected() {}

bool CRoseServer::OnAccept() { return true; }

void CRoseServer::OnAccepted(tcp::socket _sock) {
  logger_->warn(
      "CRoseServer::OnAccepted called! You should really overload this "
      "function.");
  if (_sock.is_open()) {
    std::string _address = _sock.remote_endpoint().address().to_string();

    if (IsISCServer() == false) {
      std::lock_guard<std::mutex> lock(client_list_mutex_);
      CRoseClient* nClient = new CRoseClient(std::move(_sock));
      nClient->SetId(
          std::distance(std::begin(client_list_), std::end(client_list_)));
      logger_->notice("[{}] Client connected from: {}", nClient->GetId(),
                      _address.c_str());
      client_list_.push_front(nClient);
    } else {
      std::lock_guard<std::mutex> lock(isc_list_mutex_);
      CRoseISC* nClient = new CRoseISC(std::move(_sock));
      nClient->SetId(std::distance(std::begin(isc_list_), std::end(isc_list_)));
      logger_->notice("[{}] Server connected from: {}", nClient->GetId(),
                      _address.c_str());
      isc_list_.push_front(nClient);
    }
  }
}

void CRoseServer::SendPacket(const IObject* sender, eSendType type, CRosePacket &_buffer) {
  std::lock_guard<std::mutex> lock(client_list_mutex_);
  switch(type)
  {
    case eSendType::EVERYONE:
    {
      for (auto& client : client_list_) {
        client->Send(_buffer);
      }
      break;
    }
    case eSendType::EVERYONE_BUT_ME:
    {
      for (auto& client : client_list_) {
        if( client != sender )
          client->Send(_buffer);
      }
      break;
    }
    case eSendType::NEARBY:
    {
      for (auto& client : client_list_) {
        if( client->IsNearby(sender) == true )
          client->Send(_buffer);
      }
      break;
    }
    case eSendType::NEARBY_BUT_ME:
    {
      for (auto& client : client_list_) {
        if( client != sender && client->IsNearby(sender) == true )
          client->Send(_buffer);
      }
      break;
    }
    default:
      break;
  }
}

}

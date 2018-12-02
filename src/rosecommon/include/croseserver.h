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

#ifndef _CROSE_NETWORK_H_
#define _CROSE_NETWORK_H_

#include <forward_list>
#include "inetwork.h"
#include "croseclient.h"
#include <memory>

namespace RoseCommon {

class CRoseServer : public CRoseSocket {
 public:
  CRoseServer(bool _iscServer = false);
  virtual ~CRoseServer();

  bool IsISCServer() const { return isc_server_; }

  std::forward_list<std::shared_ptr<CRoseClient>>& GetClientList() {
    return client_list_;
  }
  std::forward_list<std::shared_ptr<CRoseClient>>& GetISCList() { return isc_list_; }
  std::mutex& GetClientListMutex() { return client_list_mutex_; }
  std::mutex& GetISCListMutex() { return isc_list_mutex_; }

  enum class eSendType : uint8_t {
    EVERYONE,
    EVERYONE_BUT_ME,
    EVERYONE_BUT_ME_ON_MAP,
    NEARBY,
    NEARBY_BUT_ME,
  };

  //template <typename Func>
  //void sendPacket(Entity sender, eSendType type, CRosePacket&& _buffer, Func&& nearby_function);

  //template <typename Func>
  //void sendPacket(Entity sender, eSendType type, const CRosePacket& _buffer, Func&& nearby_function);

  void set_socket(std::unique_ptr<Core::INetwork> _val, int socket_id = static_cast<int>(SocketType::Client),
                  [[maybe_unused]] bool is_server = false) override {
    socket_[socket_id] = std::move(_val);
    socket_[socket_id]->registerOnAccepted(std::bind(&CRoseServer::OnAccepted, this, std::placeholders::_1));
  };

 protected:
  // Callback functions
  virtual void OnAccepted(std::unique_ptr<Core::INetwork> _sock) ;

  bool isc_server_;
  std::forward_list<std::shared_ptr<CRoseClient>> client_list_;
  std::forward_list<std::shared_ptr<CRoseClient>> isc_list_;
  std::mutex client_list_mutex_;
  std::mutex isc_list_mutex_;
};

/*template <typename Func>
void CRoseServer::sendPacket(Entity sender, eSendType type, CRosePacket&& _buffer, Func&& nearby) {
  std::lock_guard<std::mutex> lock(client_list_mutex_);
  switch(type)
  {
    case eSendType::EVERYONE:
    {
      for (auto& client : client_list_) {
          if (client->isLoggedIn())
              client->send(_buffer);
      }
      break;
    }
    case eSendType::EVERYONE_BUT_ME:
    {
      for (auto& client : client_list_) {
        if(client->isLoggedIn() && client->getEntity() != sender)
          client->send(_buffer);
      }
      break;
    }
    case eSendType::EVERYONE_BUT_ME_ON_MAP:
        for (auto &client : client_list_)
            if (client->isLoggedIn() && client->getEntity() != sender && isOnMap(client->getEntity()))
                client->send(_buffer);
        break;
    case eSendType::NEARBY:
    {
      for (auto& client : client_list_) {
        if(client->isLoggedIn() && isOnMap(client->getEntity()) && nearby(client->getEntity(), sender))
          client->send(_buffer);
      }
      break;
    }
    case eSendType::NEARBY_BUT_ME:
    {
      for (auto& client : client_list_) {
        if(client->isLoggedIn() && isOnMap(client->getEntity()) && client->getEntity() != sender && nearby(client->getEntity(), sender))
          client->send(_buffer);
      }
      break;
    }
    default:
      break;
  }
}*/

//template <typename Func>
//void CRoseServer::sendPacket(Entity sender, eSendType type, const CRosePacket& _buffer, Func&& nearby) {
//    sendPacket(sender, type, std::move(_buffer), std::forward<Func>(nearby));
//}

}

#endif

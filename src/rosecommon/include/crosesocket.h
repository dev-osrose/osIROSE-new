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

#ifndef _CROSESOCKET_H_
#define _CROSESOCKET_H_

#include <spdlog/spdlog.h>
#include "logconsole.h"
#include "crosecrypt.h"
#include "inetwork.h"
#include "crosepacket.h"
#include "isccommon.h"
#include <queue>
#include <array>

namespace RoseCommon {

enum SocketType : unsigned int {
  Client = 0,
  NorthWestMap,  NorthMap,   NorthEastMap,
  WestMap,      CurrentMap, EastMap,
  SouthWestMap, SouthMap,   SouthEastMap,
  MaxSockets 
};

class CRoseSocket {
 public:
  CRoseSocket();
  CRoseSocket(std::unique_ptr<Core::INetwork> _sock);
  CRoseSocket(std::unique_ptr<Core::INetwork> _sock, bool is_server,
              int socket_id = static_cast<int>(SocketType::Client));
  virtual ~CRoseSocket();

  virtual void set_socket(std::unique_ptr<Core::INetwork> _val, int socket_id = static_cast<int>(SocketType::Client),
                          bool is_server = false) 
  {
    socket_[socket_id] = std::move(_val);
    socket_[socket_id]->set_socket_id(socket_id);

    if(true == is_server)
    {
      socket_[socket_id]->registerOnReceived(std::bind(&CRoseSocket::onServerReceived, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
      socket_[socket_id]->registerOnSend(std::bind(&CRoseSocket::onServerSend, this, std::placeholders::_1, std::placeholders::_2));
      socket_[socket_id]->registerOnDisconnected(std::bind(&CRoseSocket::onServerDisconnected, this));
    }
    else
    {
      socket_[socket_id]->registerOnReceived(std::bind(&CRoseSocket::onReceived, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
      socket_[socket_id]->registerOnSend(std::bind(&CRoseSocket::onSend, this, std::placeholders::_1, std::placeholders::_2));
      socket_[socket_id]->registerOnDisconnected(std::bind(&CRoseSocket::onDisconnected, this));
    }
  };

  virtual bool send(const CRosePacket& _buffer, int socket_id = static_cast<int>(SocketType::Client));
  virtual bool send(std::unique_ptr<uint8_t[]> _buffer, int socket_id = static_cast<int>(SocketType::Client));

  virtual uint32_t get_obj_id(int socket_id = static_cast<int>(SocketType::Client)) const {
    return socket_[socket_id]->get_id();
  }

  bool init(std::string _ip, uint16_t _port, int socket_id = static_cast<int>(SocketType::Client)) {
    return socket_[socket_id]->init(_ip, _port);
  }
  bool listen(int socket_id = static_cast<int>(SocketType::Client)) {
    return socket_[socket_id]->listen();
  }
  bool connect(int socket_id = static_cast<int>(SocketType::Client)) {
    return socket_[socket_id]->connect();
  }
  bool is_active(int socket_id = static_cast<int>(SocketType::Client)) const {
    return socket_[socket_id]->is_active();
  }
  void set_active(bool _val, int socket_id = static_cast<int>(SocketType::Client)) {
    socket_[socket_id]->set_active(_val);
  }
  void set_type(uint32_t _val, int socket_id = static_cast<int>(SocketType::Client)) {
    socket_[socket_id]->set_type(_val);
  }
  uint32_t get_id(int socket_id = static_cast<int>(SocketType::Client)) const {
    return socket_[socket_id]->get_id();
  }
  Isc::ServerType get_type(int socket_id = static_cast<int>(SocketType::Client)) const {
    return static_cast<Isc::ServerType>(socket_[socket_id]->get_type());
  }
  uint16_t get_port(int socket_id = static_cast<int>(SocketType::Client)) const {
    return socket_[socket_id]->get_port();
  }
  std::string get_address(int socket_id = static_cast<int>(SocketType::Client)) const {
    return socket_[socket_id]->get_address();
  }
  std::chrono::steady_clock::time_point get_update_time(int socket_id = static_cast<int>(SocketType::Client)) const {
    return socket_[socket_id]->get_update_time();
  }
  std::string get_name(int socket_id = static_cast<int>(SocketType::Client)) const {
    return socket_[socket_id]->get_name();
  }

  virtual void set_name(const std::string _val, int socket_id = static_cast<int>(SocketType::Client)) {
    socket_[socket_id]->set_name(_val);
  }

  virtual void set_id(uint32_t _val) { socket_[static_cast<int>(SocketType::Client)]->set_id(_val); }
  virtual void set_update_time(std::chrono::steady_clock::time_point _val,
                               int socket_id = static_cast<int>(SocketType::Client)) {
      socket_[socket_id]->set_update_time(_val);
  }
  virtual bool disconnect(int socket_id = static_cast<int>(SocketType::Client)) {
    return socket_[socket_id]->disconnect();
  }
  virtual bool shutdown(bool _final = false, int socket_id = static_cast<int>(SocketType::Client)) {
    return socket_[socket_id]->shutdown(_final);
  }
  virtual void start_recv(int socket_id = static_cast<int>(SocketType::Client)) {
    socket_[socket_id]->recv_data();
  }

  std::shared_ptr<spdlog::logger> logger_;

 protected:
  // Callback functions
  virtual void onDisconnected();
  virtual bool onReceived(uint16_t socket_id_, uint16_t& packet_size_, uint8_t* buffer_) ;
  virtual bool onSend(uint16_t socket_id_, [[maybe_unused]] uint8_t* _buffer) ;
  virtual bool handlePacket(uint8_t* _buffer) ;
  
  virtual void onServerDisconnected();
  virtual bool onServerReceived(uint16_t socket_id_, uint16_t& packet_size_, uint8_t* buffer_) ;
  virtual bool onServerSend(uint16_t socket_id_, [[maybe_unused]] uint8_t* _buffer) ;
  virtual bool handleServerPacket([[maybe_unused]] uint8_t* _buffer) ;

  std::array<PacketCodec, static_cast<int>(SocketType::MaxSockets)> crypt_;
  std::array<std::unique_ptr<Core::INetwork>, static_cast<int>(SocketType::MaxSockets)> socket_;

  std::mutex recv_mutex_;
  std::queue<std::unique_ptr<uint8_t[]>> recv_queue_;
};

}

#endif

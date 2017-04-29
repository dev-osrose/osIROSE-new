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
#include <queue>

namespace RoseCommon {

class CRoseSocket {
 public:
  CRoseSocket();
  CRoseSocket(std::unique_ptr<Core::INetwork> _sock);
  virtual ~CRoseSocket();

  virtual void set_socket(std::unique_ptr<Core::INetwork> _val) {
   socket_ = std::move(_val);
   socket_->registerOnReceived(std::bind(&CRoseSocket::OnReceived, this, std::placeholders::_1, std::placeholders::_2));
   socket_->registerOnSend(std::bind(&CRoseSocket::OnSend, this, std::placeholders::_1));
   socket_->registerOnDisconnected(std::bind(&CRoseSocket::OnDisconnected, this));
  };

  virtual bool send(CRosePacket &_buffer);
  virtual bool send(std::unique_ptr<uint8_t[]> _buffer);

  virtual uint32_t get_obj_id() const {
    return socket_->get_id();
  }

  bool init(std::string _ip, uint16_t _port) { return socket_->init(_ip, _port); }
  bool listen() { return socket_->listen(); }
  bool connect() { return socket_->connect();  }
  bool is_active() const { return socket_->is_active(); }
  void set_active(bool _val) { socket_->set_active(_val); }
  void set_type(uint32_t _val) { socket_->set_type(_val); }
  uint32_t get_id() const { return socket_->get_id(); }
  uint32_t get_type() const { return socket_->get_type(); }
  uint16_t get_port() const { return socket_->get_port(); }
  std::string get_address() const { return socket_->get_address(); }
  std::chrono::steady_clock::time_point get_update_time() const { return socket_->get_update_time(); }


  virtual void set_id(uint32_t _val) { socket_->set_id(_val); }
  virtual void set_update_time(std::chrono::steady_clock::time_point _val) { socket_->set_update_time(_val); }
  virtual bool disconnect() { return socket_->disconnect(); }
  virtual bool shutdown(bool _final = false) { return socket_->shutdown(_final); }

  std::shared_ptr<spdlog::logger> logger_;

 protected:
  // Callback functions
  virtual void OnDisconnected();
  virtual bool OnReceived(uint16_t& packet_size_, uint8_t* buffer_) ;
  virtual bool OnSend(uint8_t* _buffer) ;
  virtual bool HandlePacket(uint8_t* _buffer) ;

  PacketCodec crypt_;
  std::unique_ptr<Core::INetwork> socket_;

  std::mutex recv_mutex_;
  std::queue<std::unique_ptr<uint8_t[]>> recv_queue_;
};

}

#endif

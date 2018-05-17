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

namespace RoseCommon {

class CRoseSocket {
 public:
  CRoseSocket();
  CRoseSocket(std::unique_ptr<Core::INetwork> _sock);
  CRoseSocket(std::unique_ptr<Core::INetwork> _sock, bool is_server);
  virtual ~CRoseSocket();

  virtual void set_socket(std::unique_ptr<Core::INetwork> _val, bool is_server = false) 
  {
    if(true == is_server)
    {
      server_socket_ = std::move(_val);
      server_socket_->registerOnReceived(std::bind(&CRoseSocket::OnServerReceived, this, std::placeholders::_1, std::placeholders::_2));
      server_socket_->registerOnSend(std::bind(&CRoseSocket::OnServerSend, this, std::placeholders::_1));
      server_socket_->registerOnDisconnected(std::bind(&CRoseSocket::OnServerDisconnected, this));
    }
    else
    {
      socket_ = std::move(_val);
      socket_->registerOnReceived(std::bind(&CRoseSocket::OnReceived, this, std::placeholders::_1, std::placeholders::_2));
      socket_->registerOnSend(std::bind(&CRoseSocket::OnSend, this, std::placeholders::_1));
      socket_->registerOnDisconnected(std::bind(&CRoseSocket::OnDisconnected, this));
    }
  };

  virtual bool send(CRosePacket &_buffer, bool to_server = false);
  virtual bool send(std::unique_ptr<uint8_t[]> _buffer, bool to_server = false);

  virtual uint32_t get_obj_id(bool is_server = false) const {
    if(is_server == true)
      return server_socket_->get_id();
    return socket_->get_id();
  }

  bool init(std::string _ip, uint16_t _port, bool is_server = false) { if(is_server == true) return server_socket_->init(_ip, _port); return socket_->init(_ip, _port); }
  bool listen(bool is_server = false) { if(is_server == true) return server_socket_->listen(); return socket_->listen(); }
  bool connect(bool is_server = false) { if(is_server == true) return server_socket_->connect(); return socket_->connect();  }
  bool is_active(bool is_server = false) const { if(is_server == true) return server_socket_->is_active(); return socket_->is_active(); }
  void set_active(bool _val, bool is_server = false) { if(is_server == true) server_socket_->set_active(_val); else socket_->set_active(_val); }
  void set_type(uint32_t _val, bool is_server = false) { if(is_server == true) server_socket_->set_type(_val); else socket_->set_type(_val); }
  uint32_t get_id(bool is_server = false) const { if(is_server == true) return server_socket_->get_id(); return socket_->get_id(); }
  Isc::ServerType get_type(bool is_server = false) const { if(is_server == true) return static_cast<Isc::ServerType>(server_socket_->get_type()); return static_cast<Isc::ServerType>(socket_->get_type()); }
  uint16_t get_port(bool is_server = false) const { if(is_server == true) return server_socket_->get_port(); return socket_->get_port(); }
  std::string get_address(bool is_server = false) const { if(is_server == true) return server_socket_->get_address(); return socket_->get_address(); }
  std::chrono::steady_clock::time_point get_update_time(bool is_server = false) const { if(is_server == true) return server_socket_->get_update_time(); return socket_->get_update_time(); }


  virtual void set_id(uint32_t _val) { socket_->set_id(_val); }
  virtual void set_update_time(std::chrono::steady_clock::time_point _val, bool is_server = false) { if(is_server == true) server_socket_->set_update_time(_val); else socket_->set_update_time(_val); }
  virtual bool disconnect(bool is_server = false) { if(is_server == true) return server_socket_->disconnect(); return socket_->disconnect(); }
  virtual bool shutdown(bool _final = false, bool is_server = false) { if(is_server == true) return server_socket_->shutdown(_final); return socket_->shutdown(_final); }
  virtual void start_recv(bool is_server = false) { if(is_server == true) server_socket_->recv_data(); else socket_->recv_data(); }

  std::shared_ptr<spdlog::logger> logger_;

 protected:
  // Callback functions
  virtual void OnDisconnected();
  virtual bool OnReceived(uint16_t& packet_size_, uint8_t* buffer_) ;
  virtual bool OnSend([[maybe_unused]] uint8_t* _buffer) ;
  virtual bool HandlePacket(uint8_t* _buffer) ;
  
  virtual void OnServerDisconnected();
  virtual bool OnServerReceived(uint16_t& packet_size_, uint8_t* buffer_) ;
  virtual bool OnServerSend([[maybe_unused]] uint8_t* _buffer) ;
  virtual bool HandleServerPacket([[maybe_unused]] uint8_t* _buffer) ;

  PacketCodec crypt_;
  std::unique_ptr<Core::INetwork> socket_;
  std::unique_ptr<Core::INetwork> server_socket_;

  std::mutex recv_mutex_;
  std::queue<std::unique_ptr<uint8_t[]>> recv_queue_;
};

}

#endif

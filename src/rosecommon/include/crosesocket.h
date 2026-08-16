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
#include <chrono>
#include <thread>

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
  CRoseSocket(std::unique_ptr<Core::INetwork> _sock, bool is_server);
  virtual ~CRoseSocket();

  virtual void set_socket(std::unique_ptr<Core::INetwork> _val,
                          bool is_server = false) 
  {
    socket_ = std::move(_val);
    socket_->set_socket_id(0);

    if(true == is_server)
    {
      socket_->registerOnReceived(std::bind(&CRoseSocket::onServerReceived, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
      socket_->registerOnSend(std::bind(&CRoseSocket::onServerSend, this, std::placeholders::_1, std::placeholders::_2));
      socket_->registerOnDisconnected(std::bind(&CRoseSocket::onServerDisconnected, this));
    }
    else
    {
      socket_->registerOnReceived(std::bind(&CRoseSocket::onReceived, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
      socket_->registerOnSend(std::bind(&CRoseSocket::onSend, this, std::placeholders::_1, std::placeholders::_2));
      socket_->registerOnDisconnected(std::bind(&CRoseSocket::onDisconnected, this));
    }
  };

  virtual bool send(const CRosePacket& _buffer);
  virtual bool send(std::unique_ptr<uint8_t[]> _buffer);

  [[nodiscard]] virtual uint32_t get_obj_id() const {
    return socket_->get_id();
  }

  bool init(std::string _ip, uint16_t _port) {
    return socket_->init(_ip, _port);
  }
  bool listen() {
    return socket_->listen();
  }
  bool connect() {
    return socket_->connect();
  }
  /*!
   * \brief Connect, then wait until the socket is ready to be read from.
   *
   * connect() returns as soon as the TCP session is up. In a TLS build the
   * handshake is still in flight at that point, and asio's ssl::stream cannot
   * service a read mid-handshake - posting one aborts the handshake with
   * operation_aborted, which closes the transport without a close_notify and
   * leaves the peer logging a truncated stream. Any caller that follows a
   * connect with start_recv() must use this instead.
   *
   * \return false if the connect failed outright, or if the handshake had not
   *         resolved within \a _timeout.
   */
  bool connect_and_wait(std::chrono::milliseconds _timeout = std::chrono::seconds(10)) {
    return socket_->connect_and_wait(_timeout);
  }
#ifdef USE_SSL
  /// Install the TLS material this socket terminates connections with.
  /// Must be called before listen(); listen() fails without it.
  bool enable_ssl_server(const Core::SslServerConfig& _cfg) {
    return socket_->enable_ssl_server(_cfg);
  }
  /// Install the TLS material this socket connects out with.
  /// Must be called before connect().
  bool enable_ssl_client(const Core::SslClientConfig& _cfg) {
    return socket_->enable_ssl_client(_cfg);
  }
#endif
  [[nodiscard]] bool is_active() const {
    return socket_->is_active();
  }
  void set_active(bool _val) {
    socket_->set_active(_val);
  }
  void set_type(uint32_t _val) {
    socket_->set_type(_val);
  }
  [[nodiscard]] uint32_t get_id() const {
    return socket_->get_id();
  }
  [[nodiscard]] Isc::ServerType get_type() const {
    return static_cast<Isc::ServerType>(socket_->get_type());
  }
  [[nodiscard]] uint16_t get_port() const {
    return socket_->get_port();
  }
  [[nodiscard]] std::string get_address() const {
    return socket_->get_address();
  }
  [[nodiscard]] std::chrono::steady_clock::time_point get_update_time() const {
    return socket_->get_update_time();
  }
  [[nodiscard]] std::string get_name() const {
    return socket_->get_name();
  }

  virtual void set_name(const std::string _val) {
    socket_->set_name(_val);
  }

  virtual void set_id(uint32_t _val) { socket_->set_id(_val); }
  virtual void set_update_time(std::chrono::steady_clock::time_point _val) {
      socket_->set_update_time(_val);
  }
  virtual bool disconnect() {
    return socket_->disconnect();
  }
  virtual bool shutdown(bool _final = false) {
    return socket_->shutdown(_final);
  }
  virtual void start_recv() {
    socket_->recv_data();
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

  PacketCodec crypt_;
  std::unique_ptr<Core::INetwork> socket_;

  std::mutex recv_mutex_;
  std::queue<std::unique_ptr<uint8_t[]>> recv_queue_;
};

}

#endif

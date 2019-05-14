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

#include <ctime>
#include "crosesocket.h"
#include "epackettype.h"
#include "srv_accept_reply.h"
#include "srv_screen_shot_time_reply.h"

namespace RoseCommon {

CRoseSocket::CRoseSocket() : crypt_() {
  logger_ = Core::CLog::GetLogger(Core::log_type::NETWORK).lock();

  socket_[static_cast<int>(SocketType::Client)] = nullptr;
}

CRoseSocket::CRoseSocket(std::unique_ptr<Core::INetwork> _sock) : crypt_() {
  logger_ = Core::CLog::GetLogger(Core::log_type::NETWORK).lock();

  socket_[static_cast<int>(SocketType::Client)] = std::move(_sock);
  socket_[static_cast<int>(SocketType::Client)]->set_socket_id(static_cast<int>(SocketType::Client));
  socket_[static_cast<int>(SocketType::Client)]->registerOnReceived(std::bind(&CRoseSocket::onReceived, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  socket_[static_cast<int>(SocketType::Client)]->registerOnSend(std::bind(&CRoseSocket::onSend, this, std::placeholders::_1, std::placeholders::_2));
  socket_[static_cast<int>(SocketType::Client)]->registerOnDisconnected(std::bind(&CRoseSocket::onDisconnected, this));

  socket_[static_cast<int>(SocketType::Client)]->reset_internal_buffer();
}

CRoseSocket::CRoseSocket(std::unique_ptr<Core::INetwork> _sock, bool is_server, int socket_id)
    : crypt_() {
  logger_ = Core::CLog::GetLogger(Core::log_type::NETWORK).lock();

  socket_[socket_id] = std::move(_sock);
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

  socket_[socket_id]->reset_internal_buffer();
}

CRoseSocket::~CRoseSocket() {
  for(unsigned int idx = 0; idx < SocketType::MaxSockets; ++idx)
  {
    if(socket_[idx])
      socket_[idx]->shutdown(true);
  }
  logger_.reset();
}

bool CRoseSocket::send(const CRosePacket& _buffer, int socket_id) {
  return CRoseSocket::send(_buffer.getPacked(), socket_id);
}

bool CRoseSocket::send(std::unique_ptr<uint8_t[]> _buffer, int socket_id) {
#ifdef SPDLOG_TRACE_ON
  logger_->trace("Sending a packet on CRoseSocket: Header[{0}, 0x{1:04x}]", CRosePacket::size(_buffer.get()), static_cast<uint16_t>(CRosePacket::type(_buffer.get())));
#endif
  return socket_[socket_id]->send_data(std::move(_buffer));
}

// Callback functions
void CRoseSocket::onDisconnected() {}

// TODO The socket ids in this fuction need to be fixed.
bool CRoseSocket::onReceived(uint16_t socket_id_, uint16_t& packet_size_, uint8_t* buffer_) {
  bool rtnVal = true;
  ///*
  if (packet_size_ == 6) {
#ifndef DISABLE_CRYPT
    packet_size_ = crypt_[socket_id_].decodeClientHeader(reinterpret_cast<unsigned char*>(buffer_));
#else
    packet_size_ = reinterpret_cast<uint16_t*>(buffer_)[0];
#endif

    if (packet_size_ < 6 || packet_size_ > MAX_PACKET_SIZE) {
      logger_->debug("Client sent incorrect block header");
      socket_[socket_id_]->reset_internal_buffer();
      return false;
    }

    if (packet_size_ > 6) return true;
  }

// decrypt packet now
#ifndef DISABLE_CRYPT
  if (!crypt_[socket_id_].decodeClientBody(reinterpret_cast<unsigned char*>(buffer_))) {
    // ERROR!!!
    logger_->debug( "Client sent illegal block" );
    socket_[socket_id_]->reset_internal_buffer();
    return false;
  }
#endif

  logger_->trace("Received a packet on CRoseSocket {2}: Header[{0}, 0x{1:04x}]", CRosePacket::size(buffer_), (uint16_t)CRosePacket::type(buffer_), get_id());
#ifdef SPDLOG_TRACE_ON
  fmt::MemoryWriter out;
  for(int i = 0; i < CRosePacket::size(buffer_); i++)
    out.write("0x{0:02x} ", buffer_[i]);
  logger_->trace("{}", out.c_str());
#endif

  auto res = std::make_unique<uint8_t[]>( CRosePacket::size(buffer_) );
  std::memcpy(res.get(), buffer_, CRosePacket::size(buffer_));

  recv_mutex_.lock();
  recv_queue_.push(std::move(res));
  recv_mutex_.unlock();

  socket_[socket_id_]->dispatch([this, socket_id_]() {
    if (true == socket_[socket_id_]->is_active()) {
          recv_mutex_.lock();
          bool recv_empty = recv_queue_.empty();

          if(recv_empty == false)
          {
            bool rtnVal = true;
            std::unique_ptr<uint8_t[]> _buffer = std::move(recv_queue_.front());
            recv_queue_.pop();
            recv_mutex_.unlock();

            rtnVal = handlePacket(_buffer.get());
            _buffer.reset(nullptr);

            if(rtnVal == false) {
              // Abort connection
              logger_->debug("handlePacket returned false, disconnecting client.");
              socket_[socket_id_]->shutdown();
              // TODO: if this happens, we should disconnect ALL of the sockets.
            }
          }
          else {
            recv_mutex_.unlock();
          }
        }
      });

  socket_[socket_id_]->reset_internal_buffer();
  //*/
  return rtnVal;
}

bool CRoseSocket::onSend(uint16_t socket_id_, [[maybe_unused]] uint8_t* _buffer) {
  (void)_buffer;
#ifndef DISABLE_CRYPT
  crypt_[socket_id_].encodeServerPacket(_buffer);
#endif
  return true;
}

bool CRoseSocket::handlePacket(uint8_t* _buffer) {
  logger_->trace("CRoseSocket::handlePacket start");
  switch (CRosePacket::type(_buffer)) {
    case ePacketType::PAKCS_ALIVE: {
#ifdef STRESS_TEST
      auto packet =
          std::unique_ptr<CRosePacket>(new CRosePacket(ePacketType::PAKCS_ALIVE));
      send(*packet);
#endif
      //return CNetwork_Asio::handlePacket(_buffer);
      break;
    }
#ifdef STRESS_TEST
    case ePacketType::STRESS: {
      std::unique_ptr<CRosePacket> packet( new CRosePacket(_buffer) );
      send(*packet);
      break;
    }
#endif
    case ePacketType::PAKCS_ACCEPT_REQ: {
      // Encryption stuff
      auto packet = Packet::SrvAcceptReply::create(Packet::SrvAcceptReply::ACCEPTED, std::time(nullptr));
      send(packet);
      break;
    }
    case ePacketType::PAKCS_SCREEN_SHOT_TIME_REQ: {
      auto packet = Packet::SrvScreenShotTimeReply::create(0, 0, 0, 0, 0);
      send(packet);
      break;
    }
    default: {
      logger_->warn("Unknown Packet Type: 0x{0:04x}", static_cast<uint16_t>(CRosePacket::type(_buffer)));
      return false;
    }
  }
  logger_->trace("CRoseSocket::handlePacket end");
  return true;
}

// Server Callback functions
void CRoseSocket::onServerDisconnected() {}

//TODO The socket ids in this fuction need to be fixed.
bool CRoseSocket::onServerReceived(uint16_t socket_id_, uint16_t& packet_size_, uint8_t* buffer_) {
  logger_->trace("CRoseSocket::onServerReceived start");
  bool rtnVal = true;
  ///*
  if (packet_size_ == 6) {
#ifndef DISABLE_CRYPT
    packet_size_ = crypt_[socket_id_].decodeServerHeader(reinterpret_cast<unsigned char*>(buffer_));
#else
    packet_size_ = reinterpret_cast<uint16_t*>(buffer_)[0];
#endif

    if (packet_size_ < 6 || packet_size_ > MAX_PACKET_SIZE) {
      logger_->debug("Client sent incorrect block header");
      socket_[socket_id_]->reset_internal_buffer();
      return false;
    }

    if (packet_size_ > 6) return true;
  }

// decrypt packet now
#ifndef DISABLE_CRYPT
  if (!crypt_[socket_id_].decodeServerBody(reinterpret_cast<unsigned char*>(buffer_))) {
    // ERROR!!!
    logger_->debug( "Client sent illegal block" );
    socket_[socket_id_]->reset_internal_buffer();
    return false;
  }
#endif

  logger_->trace("Received a packet on CRoseSocket {2}: Header[{0}, 0x{1:04x}]", CRosePacket::size(buffer_), (uint16_t)CRosePacket::type(buffer_), get_id());
#ifdef SPDLOG_TRACE_ON
  fmt::MemoryWriter out;
  for(int i = 0; i < CRosePacket::size(buffer_); i++)
    out.write("0x{0:02x} ", buffer_[i]);
  logger_->trace("{}", out.c_str());
#endif

  auto res = std::make_unique<uint8_t[]>( CRosePacket::size(buffer_) );
  std::memcpy(res.get(), buffer_, CRosePacket::size(buffer_));

  recv_mutex_.lock();
  recv_queue_.push(std::move(res));
  recv_mutex_.unlock();

  socket_[socket_id_]->dispatch([this, socket_id_]() {
    if (true == socket_[socket_id_]->is_active()) {
          recv_mutex_.lock();
          bool recv_empty = recv_queue_.empty();

          if(recv_empty == false)
          {
            bool rtnVal = true;
            std::unique_ptr<uint8_t[]> _buffer = std::move(recv_queue_.front());
            recv_queue_.pop();

            rtnVal = handleServerPacket(_buffer.get());
            _buffer.reset(nullptr);

            if(rtnVal == false) {
              // Abort connection
              logger_->debug("handlePacket returned false, disconnecting client.");
              socket_[socket_id_]->shutdown();
            }
          }
          recv_mutex_.unlock();
        }
      });

  socket_[socket_id_]->reset_internal_buffer();
  //*/
  return rtnVal;
}

bool CRoseSocket::onServerSend(uint16_t socket_id_, [[maybe_unused]] uint8_t* _buffer) {
#ifndef DISABLE_CRYPT
  crypt_[socket_id_].encodeClientPacket(_buffer);
#endif
  return true;
}

bool CRoseSocket::handleServerPacket([[maybe_unused]] uint8_t* _buffer) {
  logger_->trace("CRoseSocket::handleServerPacket start");
  logger_->trace("CRoseSocket::handlePacket end");
  return true;
}

}

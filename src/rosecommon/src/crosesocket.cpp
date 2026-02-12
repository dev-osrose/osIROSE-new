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

CRoseSocket::CRoseSocket() : crypt_(), socket_(nullptr) {
  logger_ = Core::CLog::GetLogger(Core::log_type::NETWORK).lock();
}

CRoseSocket::CRoseSocket(std::unique_ptr<Core::INetwork> _sock) : crypt_(), socket_(std::move(_sock)) {
  logger_ = Core::CLog::GetLogger(Core::log_type::NETWORK).lock();

  socket_->set_socket_id(0);
  socket_->registerOnReceived(std::bind(&CRoseSocket::onReceived, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  socket_->registerOnSend(std::bind(&CRoseSocket::onSend, this, std::placeholders::_1, std::placeholders::_2));
  socket_->registerOnDisconnected(std::bind(&CRoseSocket::onDisconnected, this));

  socket_->reset_internal_buffer();
}

CRoseSocket::CRoseSocket(std::unique_ptr<Core::INetwork> _sock, bool is_server)
    : crypt_(), socket_(std::move(_sock)) {
  logger_ = Core::CLog::GetLogger(Core::log_type::NETWORK).lock();

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

  socket_->reset_internal_buffer();
}

CRoseSocket::~CRoseSocket() {
  if(socket_)
    socket_->shutdown(true);
  logger_.reset();
}

bool CRoseSocket::send(const CRosePacket& _buffer) {
  return CRoseSocket::send(_buffer.getPacked());
}

bool CRoseSocket::send(std::unique_ptr<uint8_t[]> _buffer) {
#ifdef SPDLOG_TRACE_ON
  logger_->trace("Sending a packet on CRoseSocket: Header[{0}, 0x{1:04x}]", CRosePacket::size(_buffer.get()), static_cast<uint16_t>(CRosePacket::type(_buffer.get())));
#endif
  return socket_->send_data(std::move(_buffer));
}

// Callback functions
void CRoseSocket::onDisconnected() {}

// TODO The socket ids in this fuction need to be fixed.
bool CRoseSocket::onReceived(uint16_t socket_id_, uint16_t& packet_size_, uint8_t* buffer_) {
  (void)socket_id_;
  bool rtnVal = true;
  ///*
  if (packet_size_ == 6) {
#ifndef DISABLE_CRYPT
    packet_size_ = crypt_.decodeClientHeader(reinterpret_cast<unsigned char*>(buffer_));
#else
    packet_size_ = reinterpret_cast<uint16_t*>(buffer_)[0];
#endif

    if (packet_size_ < 6 || packet_size_ > MAX_PACKET_SIZE) {
      logger_->debug("Client sent incorrect block header");
      socket_->reset_internal_buffer();
      return false;
    }

    if (packet_size_ > 6) return true;
  }

// decrypt packet now
#ifndef DISABLE_CRYPT
  if (!crypt_.decodeClientBody(reinterpret_cast<unsigned char*>(buffer_))) {
    // ERROR!!!
    logger_->debug( "Client sent illegal block" );
    socket_->reset_internal_buffer();
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

  socket_->dispatch([this]() {
    if (true == socket_->is_active()) {
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
              socket_->shutdown();
            }
          }
          else {
            recv_mutex_.unlock();
          }
        }
      });

  socket_->reset_internal_buffer();
  //*/
  return rtnVal;
}

bool CRoseSocket::onSend(uint16_t socket_id_, [[maybe_unused]] uint8_t* _buffer) {
  (void)socket_id_;
  (void)_buffer;
#ifndef DISABLE_CRYPT
  crypt_.encodeServerPacket(_buffer);
#endif
  return true;
}

bool CRoseSocket::handlePacket(uint8_t* _buffer) {
  logger_->trace("CRoseSocket::handlePacket start");
  logger_->debug("Packet arrived, Type: 0x{0:04x}", static_cast<uint16_t>(CRosePacket::type(_buffer)));
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
  (void)socket_id_;
  logger_->trace("CRoseSocket::onServerReceived start");
  bool rtnVal = true;
  ///*
  if (packet_size_ == 6) {
#ifndef DISABLE_CRYPT
    packet_size_ = crypt_.decodeServerHeader(reinterpret_cast<unsigned char*>(buffer_));
#else
    packet_size_ = reinterpret_cast<uint16_t*>(buffer_)[0];
#endif

    if (packet_size_ < 6 || packet_size_ > MAX_PACKET_SIZE) {
      logger_->debug("Client sent incorrect block header");
      socket_->reset_internal_buffer();
      return false;
    }

    if (packet_size_ > 6) return true;
  }

// decrypt packet now
#ifndef DISABLE_CRYPT
  if (!crypt_.decodeServerBody(reinterpret_cast<unsigned char*>(buffer_))) {
    // ERROR!!!
    logger_->debug( "Client sent illegal block" );
    socket_->reset_internal_buffer();
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

  socket_->dispatch([this]() {
    if (true == socket_->is_active()) {
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
              socket_->shutdown();
            }
          }
          recv_mutex_.unlock();
        }
      });

  socket_->reset_internal_buffer();
  //*/
  return rtnVal;
}

bool CRoseSocket::onServerSend(uint16_t socket_id_, [[maybe_unused]] uint8_t* _buffer) {
  (void)socket_id_;
#ifndef DISABLE_CRYPT
  crypt_.encodeClientPacket(_buffer);
#endif
  return true;
}

bool CRoseSocket::handleServerPacket([[maybe_unused]] uint8_t* _buffer) {
  logger_->trace("CRoseSocket::handleServerPacket start");
  logger_->trace("CRoseSocket::handlePacket end");
  return true;
}

}

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

#include "croseisc.h"
#include "epackettype.h"

namespace RoseCommon {

CRoseISC::CRoseISC() : CRoseClient() {
}

CRoseISC::CRoseISC(std::unique_ptr<Core::INetwork> _sock) : CRoseClient(std::move(_sock)) {
  socket_[SocketType::Client]->registerOnReceived(std::bind(&CRoseISC::onReceived, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  socket_[SocketType::Client]->registerOnSend(std::bind(&CRoseISC::onSend, this, std::placeholders::_1, std::placeholders::_2));
  socket_[SocketType::Client]->registerOnConnected(std::bind(&CRoseISC::onConnected, this));
  socket_[SocketType::Client]->registerOnShutdown(std::bind(&CRoseISC::onShutdown, this));

  socket_[SocketType::Client]->reset_internal_buffer();
}

CRoseISC::~CRoseISC() {}

void CRoseISC::onConnected() {
}

bool CRoseISC::onShutdown() { return true; }

bool CRoseISC::onReceived([[maybe_unused]] uint16_t socket_id_, uint16_t& packet_size_, uint8_t* buffer_) {
  bool rtnVal = true;
  if (packet_size_ == 6) {
    packet_size_ = (uint16_t)buffer_[0];
    if (packet_size_ < 6 || packet_size_ > MAX_PACKET_SIZE) {
      logger_->debug("Client sent incorrect block header");
      socket_[SocketType::Client]->reset_internal_buffer();
      return false;
    }

    logger_->trace("Received a packet header on CRoseISC: Header[{0}, 0x{1:04x}]", packet_size_, (uint16_t)CRosePacket::type(buffer_));

    if (packet_size_ > 6) return true;
  }

  logger_->debug("Received a packet on CRoseISC: Header[{0}, 0x{1:x}]",
                 CRosePacket::size(buffer_),
                 (uint16_t)CRosePacket::type(buffer_));

  auto res = std::make_unique<uint8_t[]>(CRosePacket::size(buffer_));
  std::memcpy(res.get(), buffer_, CRosePacket::size(buffer_));

  recv_mutex_.lock();
  recv_queue_.push(std::move(res));
  recv_mutex_.unlock();

  socket_[SocketType::Client]->dispatch([this]() {
    if (true == socket_[SocketType::Client]->is_active()) {
      recv_mutex_.lock();
      bool recv_empty = recv_queue_.empty();

      if (recv_empty == false)
      {
        bool rtnVal = true;
        std::unique_ptr<uint8_t[]> _buffer = std::move(recv_queue_.front());
        recv_queue_.pop();

        rtnVal = handlePacket(_buffer.get());
        _buffer.reset(nullptr);

        if (rtnVal == false) {
          // Abort connection
          logger_->debug("handlePacket returned false, disconnecting client.");
          socket_[SocketType::Client]->shutdown();
        }
      }
      recv_mutex_.unlock();
    }
  });

  socket_[SocketType::Client]->reset_internal_buffer();
  return rtnVal;
}

bool CRoseISC::onSend([[maybe_unused]] uint16_t socket_id_, [[maybe_unused]] uint8_t* _buffer) {
  // TODO: Encrypt the isc buffer.
  set_update_time(Core::Time::GetTickCount());
  return true;
}

bool CRoseISC::handlePacket(uint8_t* _buffer) {
  switch (CRosePacket::type(_buffer)) {
    case ePacketType::ISC_ALIVE:
      return true;
    default: {
      logger_->warn("Unknown Packet Type: 0x{0:x}",
                    static_cast<uint16_t>(CRosePacket::type( _buffer )));
      return false;
    }
  }
  return true;
}
}

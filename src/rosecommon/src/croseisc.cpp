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

CRoseISC::CRoseISC(Core::INetwork* _sock) : CRoseClient(std::move(_sock)) {
  std::function<bool(uint16_t&, uint8_t*)> fnOnReceived = std::bind(&CRoseISC::OnReceived, this, std::placeholders::_1, std::placeholders::_2);
  std::function<bool(uint8_t*)> fnOnSend = std::bind(&CRoseISC::OnSend, this, std::placeholders::_1);


  socket_->registerOnReceived(fnOnReceived);
  socket_->registerOnSend(fnOnSend);

  socket_->reset_internal_buffer();
}

CRoseISC::~CRoseISC() {}

void CRoseISC::OnConnected() {
  // Do encryption handshake here
  CRoseClient::OnConnected();
}

bool CRoseISC::OnDisconnect() { return true; }

void CRoseISC::OnDisconnected() {}

bool CRoseISC::OnReceived(uint16_t& packet_size_, uint8_t* buffer_) {
  bool rtnVal = true;
  if (packet_size_ == 6) {
    packet_size_ = (uint16_t)buffer_[0];
    if (packet_size_ < 6 || packet_size_ > MAX_PACKET_SIZE) {
      logger_->debug("Client sent incorrect block header");
      socket_->reset_internal_buffer();
      return false;
    }

//    logger_->trace("Received a packet header on CRoseISC: Header[{0}, 0x{1:04x}]", packet_size_, (uint16_t)CRosePacket::type(buffer_));

    if (packet_size_ > 6) return true;
  }

  logger_->debug("Received a packet on CRoseISC: Header[{0}, 0x{1:x}]",
                 CRosePacket::size(buffer_),
                 (uint16_t)CRosePacket::type(buffer_));

  auto res = std::unique_ptr<uint8_t[]>(new uint8_t[CRosePacket::size(buffer_)]);
    std::memcpy(res.get(), buffer_, CRosePacket::size(buffer_));

  recv_mutex_.lock();
  recv_queue_.push(std::move(res));
  recv_mutex_.unlock();

  socket_->dispatch([this]() {
        if (true == socket_->is_active()) {
          recv_mutex_.lock();
          bool recv_empty = recv_queue_.empty();
          recv_mutex_.unlock();

          if(recv_empty == false)
          {
            bool rtnVal = true;
            recv_mutex_.lock();
            std::unique_ptr<uint8_t[]> _buffer = std::move(recv_queue_.front());
            recv_queue_.pop();
            recv_mutex_.unlock();

            rtnVal = HandlePacket(_buffer.get());
            _buffer.reset(nullptr);

            if(rtnVal == false) {
              // Abort connection
              logger_->debug("HandlePacket returned false, disconnecting isc server.");
              socket_->shutdown();
            }
          }
        }
      });

  socket_->reset_internal_buffer();
  return rtnVal;
}

bool CRoseISC::OnSend(uint8_t* _buffer) {
  // TODO: Encrypt the isc buffer.
  (void)_buffer;
  return true;
}

void CRoseISC::OnSent() {}

bool CRoseISC::HandlePacket(uint8_t* _buffer) {
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

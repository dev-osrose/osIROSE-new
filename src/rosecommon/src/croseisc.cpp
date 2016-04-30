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

CRoseISC::CRoseISC() : CRoseClient() { ResetBuffer(); }

CRoseISC::CRoseISC(tcp::socket _sock) : CRoseClient(std::move(_sock)) {
  ResetBuffer();
}

CRoseISC::~CRoseISC() {}

void CRoseISC::OnConnected() {
  // Do encryption handshake here
  CRoseClient::OnConnected();
}

bool CRoseISC::OnDisconnect() { return true; }

void CRoseISC::OnDisconnected() {}

bool CRoseISC::OnReceived() {
  bool rtnVal = true;
  if (packet_size_ == 6) {
    packet_size_ = (uint16_t)buffer_[0];
    if (packet_size_ < 6 || packet_size_ > MAX_PACKET_SIZE) {
      logger_->debug() << "Client sent incorrect block header";
      ResetBuffer();
      return false;
    }

    if (packet_size_ > 6) return true;
  }

  logger_->debug("Received a packet on CRoseISC: Header[{0}, 0x{1:x}]",
                 CRosePacket::size(buffer_),
                 (uint16_t)CRosePacket::type(buffer_));
  rtnVal = HandlePacket(buffer_);
  ResetBuffer();
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
                    (uint16_t)CRosePacket::type(_buffer));
      return false;
    }
  }
  return true;
}
}

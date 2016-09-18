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
#include "croseclient.h"
#include "epackettype.h"
#include "rosepackets.h"

namespace RoseCommon {
//#define STRESS_TEST

CRoseClient::CRoseClient() : CNetwork_Asio(), crypt_() {
  ResetBuffer();
}

CRoseClient::CRoseClient(tcp::socket &&_sock) : CNetwork_Asio(), crypt_() {
  SetSocket(std::move(_sock));
  Recv();
  ResetBuffer();
}

CRoseClient::~CRoseClient() { Shutdown(); }

bool CRoseClient::Send(CRosePacket &_buffer) {
  return CRoseClient::Send(_buffer.getPacked());
}

bool CRoseClient::Send(std::unique_ptr<uint8_t[]> _buffer) {
  logger_->trace("Sending a packet on CRoseClient: Header[{0}, 0x{1:04x}]", CRosePacket::size(_buffer.get()), (uint16_t)CRosePacket::type(_buffer.get()));
  return CNetwork_Asio::Send(std::move(_buffer));
}

// Callback functions
bool CRoseClient::OnConnect() { return true; }

void CRoseClient::OnConnected() { CNetwork_Asio::OnConnected(); }

bool CRoseClient::OnDisconnect() { return true; }

void CRoseClient::OnDisconnected() {}

bool CRoseClient::OnReceive() { return true; }

bool CRoseClient::OnReceived() {
  bool rtnVal = true;
  if (packet_size_ == 6) {
#ifndef DISABLE_CRYPT
    packet_size_ = crypt_.decodeClientHeader((unsigned char*)&buffer_);
#else
    packet_size_ = buffer_[0];
#endif

    if (packet_size_ < 6 || packet_size_ > MAX_PACKET_SIZE) {
      logger_->debug("Client sent incorrect block header");
      ResetBuffer();
      return false;
    }

    if (packet_size_ > 6) return true;
  }

// decrypt packet now
#ifndef DISABLE_CRYPT
  if (!crypt_.decodeClientBody((unsigned char*)&buffer_)) {
    // ERROR!!!
    logger_->debug( "Client sent illegal block" );
    ResetBuffer();
    return false;
  }
#endif

  logger_->trace("Received a packet on CRoseClient: Header[{0}, 0x{1:04x}]", CRosePacket::size(buffer_), (uint16_t)CRosePacket::type(buffer_));
#ifdef SPDLOG_TRACE_ON
  fmt::MemoryWriter out;
  for(int i = 0; i < CRosePacket::size(buffer_); i++)
    out.write("0x{0:02x} ", buffer_[i]);
  logger_->trace("{}", out.c_str());
#endif
  rtnVal = HandlePacket(buffer_);
  ResetBuffer();

  return rtnVal;
}

bool CRoseClient::OnSend(uint8_t* _buffer) {
  (void)_buffer;
#ifndef DISABLE_CRYPT
  crypt_.encodeServerPacket(_buffer);
#endif
  return true;
}
void CRoseClient::OnSent() {}

bool CRoseClient::HandlePacket(uint8_t* _buffer) {
  switch (CRosePacket::type(_buffer)) {
    case ePacketType::PAKCS_ALIVE: {
#ifdef STRESS_TEST
      auto packet =
          std::unique_ptr<CRosePacket>(new CRosePacket(ePacketType::PAKCS_ALIVE));
      Send(*packet);
#endif
      //return CNetwork_Asio::HandlePacket(_buffer);
      break;
    }
#ifdef STRESS_TEST
    case ePacketType::STRESS: {
      std::unique_ptr<CRosePacket> packet( new CRosePacket(_buffer) );
      Send(*packet);
      break;
    }
#endif
    case ePacketType::PAKCS_ACCEPT_REQ: {
      // Encryption stuff
      auto packet = makePacket<ePacketType::PAKSS_ACCEPT_REPLY>(std::time(nullptr));
      Send(*packet);
      break;
    }
    case ePacketType::PAKCS_SCREEN_SHOT_TIME_REQ: {
      auto packet = makePacket<ePacketType::PAKSC_SCREEN_SHOT_TIME_REPLY>();
      Send(*packet);
      break;
    }
    default: {
               logger_->warn("Unknown Packet Type: 0x{0:04x}", (uint16_t)CRosePacket::type(_buffer));
      return false;
    }
  }
  return true;
}

bool CRoseClient::IsNearby(const IObject* _otherClient) const { 
  (void)_otherClient; 
  return false; 
}

}

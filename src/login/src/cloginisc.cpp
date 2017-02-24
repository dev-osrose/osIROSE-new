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

#include "cloginisc.h"
#include "iscpackets.pb.h"

using namespace RoseCommon;

CLoginISC::CLoginISC()
    : CRoseISC(), channel_count_(0), min_right_(0), test_server_(false) {}

CLoginISC::CLoginISC(int* _sock)
    : CRoseISC(std::move(_sock)),
      channel_count_(0),
      min_right_(0),
      test_server_(false) {}

bool CLoginISC::HandlePacket(uint8_t* _buffer) {
  switch (CRosePacket::type(_buffer)) {
    case ePacketType::ISC_ALIVE:
      return true;
    case ePacketType::ISC_SERVER_AUTH:
      return true;
    case ePacketType::ISC_SERVER_REGISTER:
      return ServerRegister(CRosePacket(_buffer));
    case ePacketType::ISC_TRANSFER:
      return true;
    case ePacketType::ISC_SHUTDOWN:
      return ServerShutdown(CRosePacket(_buffer));
    default: { return CRoseISC::HandlePacket(_buffer); }
  }
  return true;
}

bool CLoginISC::ServerRegister(const CRosePacket& P) {
  uint16_t _size = P.size() - 6;

  iscPacket::ServerReg pServerReg;
  if (pServerReg.ParseFromArray(const_cast<CRosePacket&>(P).data(), _size) ==
      false)
    return false;  // m_Log.eicprintf( "Couldn't decode proto msg\n" );

  int16_t _type = 0;

  _type = pServerReg.type();

  // 1 == char server
  // 2 == node server
  // 3 == map master server (This is the only type the login server will care
  // about)
  // 4 == map workers/threads

  // todo: replace these numbers with the actual enum name
  if (_type == iscPacket::ServerType::CHAR) {
    server_name_ = pServerReg.name();
    network_ip_address_ = pServerReg.addr();
    network_port_ = pServerReg.port();
    min_right_ = pServerReg.accright();
    network_type_ = _type;

    this->SetType( _type );
  } else if (_type == iscPacket::ServerType::MAP_MASTER) {
    // todo: add channel connections here (_type == 3)
    tChannelInfo channel;
    channel.channelName = pServerReg.name();
    channel.ChannelID = pServerReg.id();
    channel.MinRight = pServerReg.accright();
    channel_list_.push_front(channel);
    channel_count_++;
  }

  logger_->debug( "ISC Server Type: [{}]\n",
                   GetType() );

  logger_->info("ISC Server Connected: [{}, {}, {}:{}]\n",
                  ServerType_Name(pServerReg.type()).c_str(),
                  pServerReg.name().c_str(), pServerReg.addr().c_str(),
                  pServerReg.port());
  return true;
}

bool CLoginISC::ServerShutdown(const CRosePacket& P) {
  (void)P;
  uint16_t _size = P.size() - 6;

  iscPacket::ServerShutdown pServerShutdown;
  if (pServerShutdown.ParseFromArray(const_cast<CRosePacket&>(P).data(),
                                     _size) == false)
    return false;

  channel_list_.remove_if([pServerShutdown](RoseCommon::tChannelInfo channel) {
    return channel.ChannelID == pServerShutdown.id();
  });
  return true;
}

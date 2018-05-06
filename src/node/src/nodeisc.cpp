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

#include "nodeisc.h"
#include "isc_serverregister.h"
#include "isc_shutdown.h"

using namespace RoseCommon;

NodeISC::NodeISC()
    : CRoseISC(), channel_count_(0), min_right_(0), test_server_(false) {}

NodeISC::NodeISC(std::unique_ptr<Core::INetwork> _sock)
    : CRoseISC(std::move(_sock)),
      channel_count_(0),
      min_right_(0),
      test_server_(false) {}

bool NodeISC::HandlePacket(uint8_t* _buffer) {
  logger_->trace("NodeISC::HandlePacket(uint8_t* _buffer)");
  switch (CRosePacket::type(_buffer)) {
    case ePacketType::ISC_ALIVE:
      return true;
    case ePacketType::ISC_SERVER_AUTH:
      return true;
    case ePacketType::ISC_SERVER_REGISTER:
      return ServerRegister(getPacket<ePacketType::ISC_SERVER_REGISTER>(_buffer));
    case ePacketType::ISC_TRANSFER:
      return true;
    case ePacketType::ISC_SHUTDOWN:
      return ServerShutdown(getPacket<ePacketType::ISC_SHUTDOWN>(_buffer));
    default: { return CRoseISC::HandlePacket(_buffer); }
  }
  return true;
}

bool NodeISC::ServerRegister(std::unique_ptr<IscServerRegister> P) {
  logger_->trace("NodeISC::ServerRegister(const CRosePacket& P)");

  uint8_t _type = to_underlying(P->serverType());

  // 1 == char server
  // 2 == node server
  // 3 == map master server (This is the only type the login server will care
  // about)
  // 4 == map workers/threads

  // todo: replace these numbers with the actual enum name
  if (_type == Isc::ServerType::CHAR) {
    server_name_ = P->name();
    socket_->set_address(P->addr());
    socket_->set_port(P->port());
    min_right_ = P->right();
    socket_->set_type(_type);
  } else if (_type == Isc::ServerType::MAP_MASTER) {
    // todo: add channel connections here (_type == 3)
    tChannelInfo channel;
    channel.channelName = P->name();
    channel.ChannelID = P->id();
    channel.MinRight = P->right();
    channel_list_.push_front(channel);
    channel_count_++;
  }

  logger_->debug( "ISC Server Type: [{}]\n",
    socket_->get_type() );

  logger_->info("ISC Server Connected: [{}, {}, {}:{}]\n",
                RoseCommon::Isc::serverTypeName(P->serverType()),
                  P->name(), P->addr(),
                  P->port());
  return true;
}

bool NodeISC::ServerShutdown(std::unique_ptr<IscShutdown> P) {
  channel_list_.remove_if([&](RoseCommon::tChannelInfo channel) {
    return channel.ChannelID == P->id();
  });
  return true;
}

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
#include "isc_loginreq.h"
#include "isc_serverregister.h"
#include "isc_shutdown.h"

using namespace RoseCommon;

CLoginISC::CLoginISC()
    : CRoseISC(), login_state_(eSTATE::DEFAULT), channel_count_(0), min_right_(0), test_server_(false) {}

CLoginISC::CLoginISC(std::unique_ptr<Core::INetwork> _sock)
    : CRoseISC(std::move(_sock)),
      login_state_(eSTATE::DEFAULT),
      channel_count_(0),
      min_right_(0),
      test_server_(false) {}

bool CLoginISC::HandlePacket(uint8_t* _buffer) {
  logger_->trace("CLoginISC::HandlePacket(uint8_t* _buffer)");
  switch (CRosePacket::type(_buffer)) {
    case ePacketType::ISC_ALIVE:
      return true;
    case ePacketType::ISC_SERVER_AUTH:
      return ServerAuth(getPacket<ePacketType::ISC_SERVER_AUTH>(_buffer));
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

bool CLoginISC::ServerAuth(std::unique_ptr<RoseCommon::IscLoginReq> P) {
  logger_->trace("CLoginISC::ServerAuth(const CRosePacket& P)");
  if(login_state_ != eSTATE::DEFAULT) {
    logger_->warn("ISC {} is attempting to auth multiple times.", get_id());
    return false;
  }
  std::string username_ = Core::escapeData(P->username());
  std::string clientpass = Core::escapeData(P->password());
  
  auto conn = Core::connectionPool.getConnection(Core::osirose);
  Core::AccountTable table{};
  try {
    const auto res = conn(sqlpp::select(table.id)
              .from(table).where(table.accountType == "system" and table.username == username_
                  and table.password == sqlpp::verbatim<sqlpp::varchar>(fmt::format("SHA2(CONCAT('{}', salt), 256)", clientpass))));

        if (!res.empty()) {
          login_state_ = eSTATE::LOGGEDIN;
          return true;
        } else {
          logger_->error("Char server auth from '{}' failed to auth with User: '{}' Pass: '{}'", get_address(), username_, clientpass);
        }
  } catch (const sqlpp::exception &e) {
    logger_->error("Error while accessing the database: {}", e.what());
  }
  
  return false;
}

bool CLoginISC::ServerRegister(std::unique_ptr<IscServerRegister> P) {
  logger_->trace("CLoginISC::ServerRegister(const CRosePacket& P)");
  if(login_state_ == eSTATE::DEFAULT) {
    logger_->warn("ISC {} is attempting to register before auth.", get_id());
    return false;
  }

  uint8_t _type = to_underlying(P->serverType());

  // 1 == char server
  // 2 == node server
  // 3 == map master server (This is the only type the login server will care
  // about)
  // 4 == map workers/threads

  // todo: replace these numbers with the actual enum name
  if (_type == Isc::ServerType::CHAR) {
    server_name_ = P->name();
    socket_[SocketType::Client]->set_address(P->addr());
    socket_[SocketType::Client]->set_port(P->port());
    min_right_ = P->right();
    socket_[SocketType::Client]->set_type(_type);
  } else if (_type == Isc::ServerType::MAP_MASTER) {
    // todo: add channel connections here (_type == 3)
    tChannelInfo channel;
    channel.channelName = P->name();
    channel.ChannelID = P->id();
    channel.MinRight = P->right();
    channel_list_.push_front(channel);
    channel_count_++;
  }

  this->set_name(server_name_);

  logger_->debug( "ISC Server Type: [{}]\n", socket_[SocketType::Client]->get_type());
  
  login_state_ = eSTATE::REGISTERED;

  logger_->info("ISC Server {} Connected: [{}, {}, {}:{}]\n",
                get_id(),
                RoseCommon::Isc::serverTypeName(P->serverType()),
                  P->name(), P->addr(),
                  P->port());
  return true;
}

bool CLoginISC::ServerShutdown(std::unique_ptr<IscShutdown> P) {
  // if(login_state_ != eSTATE::REGISTERED) {
  //   logger_->warn("ISC {} is attempting to shutdown before registering.", get_id());
  //   return false;
  // }
  channel_list_.remove_if([&](RoseCommon::tChannelInfo channel) {
    return channel.ChannelID == P->id();
  });
  return true;
}

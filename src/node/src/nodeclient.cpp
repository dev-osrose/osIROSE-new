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

#include "nodeclient.h"
#include "packetfactory.h"
#include "croseserver.h"
#include "srv_srvselectreply.h"
#include "epackettype.h"
#include "config.h"

using namespace RoseCommon;

namespace RoseCommon {
  REGISTER_RECV_PACKET(ePacketType::PAKLC_SRV_SELECT_REPLY, SrvSrvSelectReply)
};

NodeClient::NodeClient()
  : CRoseClient(),
    access_rights_( 0 ),
    login_state_( eSTATE::DEFAULT ),
    userid_( 0 ),
    session_id_( 0 ) {}

NodeClient::NodeClient(std::unique_ptr<Core::INetwork> _sock)
  : CRoseClient( move( _sock ) ),
    access_rights_( 0 ),
    login_state_( eSTATE::DEFAULT ),
    userid_( 0 ),
    session_id_( 0 ) {}

bool NodeClient::ServerSelect(
  std::unique_ptr<SrvSrvSelectReply> P) {

  auto& config = Core::Config::getInstance();
  auto packet = makePacket<ePacketType::PAKLC_SRV_SELECT_REPLY>(
    P->result(), P->sessionId(), P->cryptVal(),
    config.serverData().ip, config.loginServer().clientPort); // Replace this with MY ip address
  this->send( *packet );
  return true;
}

bool NodeClient::HandlePacket(uint8_t* _buffer) {
  logger_->trace( "NodeClient::HandlePacket start" );
  switch ( CRosePacket::type( _buffer ) ) {
    case ePacketType::PAKLC_SRV_SELECT_REPLY:
      return ServerSelect(
        getPacket<ePacketType::PAKLC_SRV_SELECT_REPLY>( _buffer ) );
    default:
      return CRoseClient::HandlePacket( _buffer );
  }
}


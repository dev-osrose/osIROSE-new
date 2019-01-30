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

#include "cnetwork_asio.h"
#include "nodeclient.h"
#include "packetfactory.h"
#include "croseserver.h"
#include "epackettype.h"
#include "config.h"
#include "nodesessions.h"

using namespace RoseCommon;

NodeClient::NodeClient()
  : CRoseClient(),
    session_id_( 0 ) {}

NodeClient::NodeClient(std::unique_ptr<Core::INetwork> _sock)
  : CRoseClient( move( _sock ) ),
    session_id_( 0 ) {}

bool NodeClient::serverSelectReply(Packet::SrvSrvSelectReply&& P) {
  auto& config = Core::Config::getInstance();
  //TODO: push the data into the sqlite db
  
  auto conn = connectionPoolMem.getConnection<NodeDB>();
  NodeSessionsTable table{};
  
  {
    const auto res = conn(insert_into(table).set(table.id = P.get_sessionId(), table.charip = P.get_ip(), table.charport = P.get_port()));
    logger_->debug("result of insert is {}", res);
  }
  {
    const auto res = conn(sqlpp::select(table.id, table.charip, table.charport, table.worldip, table.worldport)
              .from(table).where(table.id == P.get_sessionId()));
    if (!res.empty()) {
      const auto &row = res.front();
      logger_->debug("id: '{}' charip: '{}' charport: '{}' worldip: '{}' worldport: '{}'", row.id, row.charip, row.charport, row.worldip, row.worldport);
    }
  }
  
  auto packet = Packet::SrvSrvSelectReply::create(
    P.get_result(), P.get_sessionId(), P.get_cryptVal(),
    config.serverData().ip, config.charServer().clientPort); // Replace this with MY current ip address
    
  // Tell the client to connect to me!
  send(packet);
  return true;
}

bool NodeClient::clientAcceptReq(Packet::CliAcceptReq&& P) {
  logger_->trace( "NodeClient::clientAcceptReq start" );
  auto& config = Core::Config::getInstance();

  // Disconnect from the current server
  disconnect(RoseCommon::SocketType::CurrentMap);
  
  // Set up our socket to connect to the login server
  init(config.nodeServer().loginIp, config.nodeServer().loginPort, RoseCommon::SocketType::CurrentMap);
  
  // Actually connect to the new server
  connect(RoseCommon::SocketType::CurrentMap);
  
  // Make sure we get the data from the server
  start_recv(RoseCommon::SocketType::CurrentMap);
  
  // Send the packet to the server
  send(P, SocketType::CurrentMap);
  return true;
}

bool NodeClient::handlePacket(uint8_t* _buffer) {
  logger_->trace( "NodeClient::HandlePacket start" );
  switch ( CRosePacket::type( _buffer ) ) {
    case ePacketType::PAKCS_ACCEPT_REQ:
      return clientAcceptReq(
          Packet::CliAcceptReq::create(_buffer));
    default:
    {
      //TODO:: Make sure the socket is connected before doing this
      // Send the packet to the server
      auto res = std::make_unique<uint8_t[]>( CRosePacket::size(_buffer) );
      std::memcpy(res.get(), _buffer, CRosePacket::size(_buffer));
      send(std::move(res), SocketType::CurrentMap);
      return true;
    }
  }
}

bool NodeClient::handleServerPacket(uint8_t* _buffer) {
  logger_->trace( "NodeClient::HandleServerPacket start" );
  switch ( CRosePacket::type( _buffer ) ) {
    case ePacketType::PAKLC_SRV_SELECT_REPLY:
      return serverSelectReply(
        Packet::SrvSrvSelectReply::create(_buffer));
    default:
    {
      auto res = std::make_unique<uint8_t[]>( CRosePacket::size(_buffer) );
      std::memcpy(res.get(), _buffer, CRosePacket::size(_buffer));
      send( std::move(res), SocketType::Client );
      return true;
    }
  }
}


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

#include "cmapserver.h"
#include "cmapisc.h"
#include "cmapclient.h"
#include "epackettype.h"
#include "database.h"

using namespace RoseCommon;

CMapClient::CMapClient() 
    : CRoseClient(), 
      access_rights_(0), 
      login_state_(eSTATE::DEFAULT),
      session_id_(0),
      userid_(0),
      charid_(0) {}

CMapClient::CMapClient(tcp::socket _sock)
    : CRoseClient(std::move(_sock)),
      access_rights_(0), 
      login_state_(eSTATE::DEFAULT),
      session_id_(0),
      userid_(0),
      charid_(0) {}

bool CMapClient::HandlePacket(uint8_t* _buffer) {
  switch (CRosePacket::type(_buffer)) {
    case ePacketType::PAKCS_JOIN_SERVER_REQ:
      return JoinServerReply(getPacket<ePacketType::PAKCS_JOIN_SERVER_REQ>(
          _buffer));  // Allow client to connect
//    case ePacketType::PAKCS_CHANGE_MAP_REQ:
//      return ChangeMapReply(getPacket<ePacketType::PAKCS_CHANGE_MAP_REQ>(_buffer));
//    case ePacketType::PAKCS_LOGOUT_REQ:
//      return LogoutReply();
//    case ePacketType::PAKCS_NORMAL_CHAT:
//      return ChatReply(getPacket<ePacketType::PAKCS_NORMAL_CHAT>(_buffer));
    default:
      return CRoseClient::HandlePacket(_buffer);
  }
  return true;
}

bool CMapClient::OnReceived() { return CRoseClient::OnReceived(); }

bool CMapClient::JoinServerReply(
    std::unique_ptr<RoseCommon::CliJoinServerReq> P) {
  logger_->trace("JoinServerReply\n");
  
  if (login_state_ != eSTATE::DEFAULT) {
    logger_->warn("Client {} is attempting to login when already logged in.",
                  GetId());
    return true;
  }
  
  uint32_t sessionID = P->session_id();
  std::string password = P->password();
  
  std::unique_ptr<Core::IResult> res;
  std::string query = fmt::format("CALL GetSession({});", sessionID);

  Core::IDatabase& database = Core::databasePool.getDatabase();
  res = database.QStore(query);
  if (res != nullptr) {  // Query the DB
    if (res->size() != 0) {
      std::string pwd = "";
      res->getString("password", pwd);
      if (pwd == password) {
        login_state_ = eSTATE::LOGGEDIN;
        res->getInt("userid", userid_);
        res->getInt("charid", charid_);
        
        logger_->debug("Client {} auth OK.",
                GetId());

        auto packet = makePacket<ePacketType::PAKSC_JOIN_SERVER_REPLY>(
            SrvJoinServerReply::OK, std::time(nullptr));
        Send(*packet);

        //SEND PLAYER DATA HERE!!!!!!
        auto packet2 = makePacket<ePacketType::PAKWC_SELECT_CHAR_REPLY>( );
        packet2->setCharacter("test", 1, 20, 5200, 5200, 20, sessionID);
        Send( *packet2 );

        auto packet3 = makePacket<ePacketType::PAKWC_INVENTORY_DATA>(1000000);
        Send( *packet3 );
        
        auto packet4 = makePacket<ePacketType::PAKWC_QUEST_DATA>();
        Send( *packet4 );

        auto packet5 = makePacket<ePacketType::PAKWC_BILLING_MESSAGE>();
        Send( *packet5 );
      } else {
        logger_->debug("Client {} auth INVALID_PASS.",
                GetId());
        auto packet = makePacket<ePacketType::PAKSC_JOIN_SERVER_REPLY>(
            SrvJoinServerReply::INVALID_PASSWORD, std::time(nullptr));
        Send(*packet);
      }
    } else {
      logger_->debug("Client {} auth FAILED.",
                GetId());
      auto packet = makePacket<ePacketType::PAKSC_JOIN_SERVER_REPLY>(
          SrvJoinServerReply::FAILED, std::time(nullptr));
      Send(*packet);
    }
  }
  return true;
};
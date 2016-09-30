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

CMapClient::CMapClient(tcp::socket _sock, std::shared_ptr<EntitySystem> entitySystem)
    : CRoseClient(std::move(_sock)),
      access_rights_(0),
      login_state_(eSTATE::DEFAULT),
      session_id_(0),
      userid_(0),
      charid_(0),
      entitySystem_(entitySystem)
      {}

bool CMapClient::HandlePacket(uint8_t* _buffer) {
  switch (CRosePacket::type(_buffer)) {
    case ePacketType::PAKCS_JOIN_SERVER_REQ:
      return JoinServerReply(getPacket<ePacketType::PAKCS_JOIN_SERVER_REQ>(
          _buffer));  // Allow client to connect
    case ePacketType::PAKCS_CHANGE_MAP_REQ:
      return ChangeMapReply(
          getPacket<ePacketType::PAKCS_CHANGE_MAP_REQ>(_buffer));
    //    case ePacketType::PAKCS_LOGOUT_REQ:
    //      return LogoutReply();
    case ePacketType::PAKCS_NORMAL_CHAT:
      return ChatReply(getPacket<ePacketType::PAKCS_NORMAL_CHAT>(_buffer));
    case ePacketType::PAKCS_MOUSE_CMD:
      return MouseCmdRcv(getPacket<ePacketType::PAKCS_MOUSE_CMD>(_buffer));
    case ePacketType::PAKCS_STOP_MOVING:
      return StopMovingRcv(getPacket<ePacketType::PAKCS_STOP_MOVING>(_buffer));
    default: {
      // logger_->debug( "cmapclient default handle: 0x{1:04x}",
      // (uint16_t)CRosePacket::type(_buffer) );
      return CRoseClient::HandlePacket(_buffer);
    }
  }
  return true;
}

CMapClient::~CMapClient() {
    entitySystem_->saveCharacter(charid_, entity_);
    entity_.destroy();
}

bool CMapClient::OnReceived() { return CRoseClient::OnReceived(); }

bool CMapClient::JoinServerReply(
    std::unique_ptr<RoseCommon::CliJoinServerReq> P) {
  logger_->trace("CMapClient::JoinServerReply()");

  if (login_state_ != eSTATE::DEFAULT) {
    logger_->warn("Client {} is attempting to login when already logged in.",
                  GetId());
    return true;
  }

  uint32_t sessionID = P->session_id();
  std::string password = P->password();

  std::unique_ptr<Core::IResult> res, itemres;
  std::string query = fmt::format("CALL get_session({}, '{}');", sessionID, password);

  Core::IDatabase& database = Core::databasePool.getDatabase();
  res = database.QStore(query);
  if (res != nullptr) {  // Query the DB
    if (res->size() != 0) {
      logger_->debug("Client {} auth OK.", GetId());
      login_state_ = eSTATE::LOGGEDIN;
      res->getInt("userid", userid_);
      res->getInt("charid", charid_);
      session_id_ = sessionID;
      bool platinium = false;
      res->getInt("platinium", platinium);
      entity_ = entitySystem_->loadCharacter(charid_, platinium);

      if (entity_) {
          auto basic =entity_.component<BasicInfo>();
          basic->tag_ = GetId();
        auto packet = makePacket<ePacketType::PAKSC_JOIN_SERVER_REPLY>(
            SrvJoinServerReply::OK, std::time(nullptr));
        Send(*packet);

        entity_.assign<SocketConnector>(this);
        // SEND PLAYER DATA HERE!!!!!!
        auto packet2 = makePacket<ePacketType::PAKWC_SELECT_CHAR_REPLY>(entity_);
        Send(*packet2);

        auto packet3 = makePacket<ePacketType::PAKWC_INVENTORY_DATA>(entity_.component<AdvancedInfo>()->zuly_);
        Send(*packet3);

        auto packet4 = makePacket<ePacketType::PAKWC_QUEST_DATA>();
        Send(*packet4);

        auto packet5 = makePacket<ePacketType::PAKWC_BILLING_MESSAGE>();
        Send(*packet5);

        auto packet6 = makePacket<ePacketType::PAKWC_PLAYER_CHAR>(entity_);
        CMapServer::SendPacket(this, CMapServer::eSendType::EVERYONE_BUT_ME, *packet6);
      } else {
          logger_->debug("Something wrong happened when creating the entity");
      }
    } else {
      logger_->debug("Client {} auth INVALID_PASS.", GetId());
      auto packet = makePacket<ePacketType::PAKSC_JOIN_SERVER_REPLY>(
          SrvJoinServerReply::INVALID_PASSWORD, 0);
      Send(*packet);
    }
   } else {
      logger_->debug("Client {} auth FAILED.", GetId());
      auto packet = makePacket<ePacketType::PAKSC_JOIN_SERVER_REPLY>(
          SrvJoinServerReply::FAILED, 0);
      Send(*packet);
    }
  return true;
};

bool CMapClient::ChangeMapReply(
    std::unique_ptr<RoseCommon::CliChangeMapReq> P) {
  logger_->trace("CMapClient::ChangeMapReply()");

  if (login_state_ != eSTATE::LOGGEDIN) {
    logger_->warn("Client {} is attempting to change map before logging in.",
                  GetId());
    return true;
  }
  (void)P;
  auto advanced = entity_.component<AdvancedInfo>();
  auto basic = entity_.component<BasicInfo>();
  auto info = entity_.component<CharacterInfo>();
  Send(*makePacket<ePacketType::PAKWC_CHANGE_MAP_REPLY>(GetId(), advanced->hp_, advanced->mp_, basic->xp_, info->penaltyXp_, std::time(nullptr), 0));
  CMapServer::SendPacket(this, CMapServer::eSendType::EVERYONE_BUT_ME,
          *makePacket<ePacketType::PAKWC_PLAYER_CHAR>(entity_));

  return true;
}

bool CMapClient::ChatReply(std::unique_ptr<RoseCommon::CliChat> P) {
  logger_->trace("CMapClient::ChatReply()");

  if (login_state_ != eSTATE::LOGGEDIN) {
    logger_->warn("Client {} is attempting to chat before logging in.",
                  GetId());
    return true;
  }

  uint16_t _charID = GetId();
  std::string _message = P->getMessage();

  auto packet = makePacket<ePacketType::PAKWC_NORMAL_CHAT>(
          _message, _charID);
  logger_->trace("client {} is sending '{}'", _charID, _message);
  CMapServer::SendPacket(this, CMapServer::eSendType::NEARBY, *packet);
  return true;
}

bool CMapClient::IsNearby(const IObject* _otherClient) const {
  (void)_otherClient;
  logger_->trace("CMapClient::IsNearby()");
  //TODO: Call the entity distance calc here
  return true;
}

bool CMapClient::MouseCmdRcv(std::unique_ptr<RoseCommon::CliMouseCmd> P) {
    logger_->trace("CMapClient::MouseCmdRcv()");
    if (login_state_ != eSTATE::LOGGEDIN) {
        logger_->warn("Client {} is attempting to issue mouse commands before logging in.", GetId());
        return true;
    }
    // TODO : set target
    entitySystem_->get<MovementSystem>().move(entity_, 0, 0);
    CMapServer::SendPacket(this, CMapServer::eSendType::EVERYONE,
            *makePacket<ePacketType::PAKWC_MOUSE_CMD>(GetId(), P->targetId(), 0, P->x(), P->y(), P->z()));
    return true;
}

bool CMapClient::StopMovingRcv(std::unique_ptr<RoseCommon::CliStopMoving> P) {
    logger_->trace("CMapClient::StopMovingRcv()");
    if (login_state_ != eSTATE::LOGGEDIN) {
        logger_->warn("Client {} is attempting to stop moving before logging in.", GetId());
        return true;
    }
    entitySystem_->get<MovementSystem>().stop(entity_, P->x(), P->y());
    return true;
}

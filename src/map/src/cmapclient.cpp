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
#include "entityComponents.h"
#include <cmath>

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
		case ePacketType::PAKCS_WHISPER_CHAT:
			return ChatWhisper(getPacket<ePacketType::PAKCS_WHISPER_CHAT>(_buffer));
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
}

bool CMapClient::OnReceived() { return CRoseClient::OnReceived(); }

void CMapClient::OnDisconnected() {
    entitySystem_->saveCharacter(charid_, entity_);
    CMapServer::SendPacket(this, CMapServer::eSendType::EVERYONE_BUT_ME, *makePacket<ePacketType::PAKWC_REMOVE_OBJECT>(entity_));
    entitySystem_->destroy(entity_);
}

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
          basic->id_ = GetId();
          basic->tag_ = GetId();
        auto packet = makePacket<ePacketType::PAKSC_JOIN_SERVER_REPLY>(
            SrvJoinServerReply::OK, std::time(nullptr));
        Send(*packet);

        entity_.assign<SocketConnector>(this);
        // SEND PLAYER DATA HERE!!!!!!
        auto packet2 = makePacket<ePacketType::PAKWC_SELECT_CHAR_REPLY>(entity_);
        Send(*packet2);

        auto packet3 = makePacket<ePacketType::PAKWC_INVENTORY_DATA>(entity_);
        Send(*packet3);

        auto packet4 = makePacket<ePacketType::PAKWC_QUEST_DATA>();
        Send(*packet4);

        auto packet5 = makePacket<ePacketType::PAKWC_BILLING_MESSAGE>();
        Send(*packet5);

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

  entitySystem_->process<CharacterGraphics, BasicInfo>([entity_ = entity_, this](Entity entity) {
          auto basic = entity.component<BasicInfo>();
          if (entity != entity_ && basic->loggedIn_)
              this->Send(*makePacket<ePacketType::PAKWC_PLAYER_CHAR>(entity));
        });

  basic->loggedIn_ = true;

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

bool CMapClient::ChatWhisper(std::unique_ptr<RoseCommon::CliWhisper> P) {
  logger_->trace("CMapClient::ChatWhisper()");

  if (login_state_ != eSTATE::LOGGEDIN) {
    logger_->warn("Client {} is attempting to whisper before logging in.",
                  GetId());
    return true;
  }

  uint16_t _charID = GetId();
  std::string _sender_name = entity_.component<BasicInfo>()->name_; 
	std::string _recipient_name = P->targetId();
	logger_->debug("first hex: {0:x}", _recipient_name[0]);
	logger_->debug("2nd to last hex: {0:x}", _recipient_name[6]);
	logger_->debug("last hex: {0:x}", _recipient_name[7]);
	_recipient_name.erase(_recipient_name.size()-1); // for some reason this string has an extra char
	std::string _message = P->message();
	CMapClient *_recipient_client = nullptr;
	entitySystem_->process<BasicInfo, SocketConnector>([this, &_recipient_client, _recipient_name] (Entity entity) {
		if(_recipient_name == entity.component<BasicInfo>()->name_) {
			_recipient_client = entity.component<SocketConnector>()->client_;
			return true;
		}
		return false;
	});
  logger_->trace("{} ({}) is whispering '{}' to {}", _sender_name, _charID,  _message, _recipient_name);
	if(_recipient_client) {
		auto packet = makePacket<ePacketType::PAKWC_WHISPER_CHAT>(_message, _sender_name);
  	_recipient_client->Send(*packet);
  	return true;
	} else {
		auto packet = makePacket<ePacketType::PAKWC_WHISPER_CHAT>(std::string(""), std::string("User cannot be found or is offline"));
		this->Send(*packet);
		return true;
	}
	return false;
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
    auto pos = entity_.component<Position>();
    float dx = pos->x_ - P->x(), dy = pos->y_ - P->y();
    entitySystem_->get<MovementSystem>().move(entity_, P->x(), P->y());
    CMapServer::SendPacket(this, CMapServer::eSendType::EVERYONE,
            *makePacket<ePacketType::PAKWC_MOUSE_CMD>(GetId(), P->targetId(), std::sqrt(dx*dx + dy*dy), P->x(), P->y(), P->z()));
    return true;
}

bool CMapClient::StopMovingRcv(std::unique_ptr<RoseCommon::CliStopMoving> P) {
    logger_->trace("CMapClient::StopMovingRcv()");
    if (login_state_ != eSTATE::LOGGEDIN) {
        logger_->warn("Client {} is attempting to stop moving before logging in.", GetId());
        return true;
    }
    entitySystem_->get<MovementSystem>().stop(entity_, P->x(), P->y());
    CMapServer::SendPacket(this, CMapServer::eSendType::EVERYONE_BUT_ME,
            *makePacket<ePacketType::PAKWC_STOP>(entity_));
    return true;
}

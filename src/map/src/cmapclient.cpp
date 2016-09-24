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
      entitySystem_(entitySystem),
      entity_(entitySystem_->create()) {}

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
    default: {
      // logger_->debug( "cmapclient default handle: 0x{1:04x}",
      // (uint16_t)CRosePacket::type(_buffer) );
      return CRoseClient::HandlePacket(_buffer);
    }
  }
  return true;
}

CMapClient::~CMapClient() {
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
  std::string query = fmt::format("CALL GetSession({});", sessionID);

  Core::IDatabase& database = Core::databasePool.getDatabase();
  res = database.QStore(query);
  if (res != nullptr) {  // Query the DB
    if (res->size() != 0) {
      std::string pwd = "";
      res->getString("password", pwd);
      if (pwd == password) {
        logger_->debug("Client {} auth OK.", GetId());
        login_state_ = eSTATE::LOGGEDIN;
        res->getInt("userid", userid_);
        res->getInt("charid", charid_);
        session_id_ = sessionID;

        query = fmt::format("CALL GetCharacter({});", charid_);
        res = database.QStore(query);
        if (res != nullptr && res->size() == 1) {
          auto packet = makePacket<ePacketType::PAKSC_JOIN_SERVER_REPLY>(
              SrvJoinServerReply::OK, std::time(nullptr));
          Send(*packet);

          uint32_t race = 0, face, hair;
          uint32_t zone = 1, revive_zone = 1;
          float pos[2] = {530000, 530000};
          uint64_t zuly = 0;
          std::string name;

          res->getString("name", name);
          res->getInt("race", race);
          res->getInt("map", zone);
          res->getInt("revive_map", revive_zone);
          res->getFloat("x", pos[0]);
          res->getFloat("y", pos[1]);
          res->getInt("face", face);
          res->getInt("hair", hair);
          // res->getInt( "zuly", zuly );

          entity_.assign<Position>(pos[0], pos[1], zone);
          entity_.assign<SocketConnector>(this);
          // SEND PLAYER DATA HERE!!!!!!
          auto packet2 = makePacket<ePacketType::PAKWC_SELECT_CHAR_REPLY>();
          packet2->setCharacter(name, race, zone, pos[0], pos[1], revive_zone,
                                sessionID);
          packet2->addEquipItem(
              SrvSelectCharReply::equipped_position::EQUIP_FACE, face);
          packet2->addEquipItem(
              SrvSelectCharReply::equipped_position::EQUIP_HAIR, hair);

          query = fmt::format("CALL GetEquipped({});", charid_);
          itemres = database.QStore(query);
          if (itemres != nullptr) {
            if (itemres->size() != 0) {
              for (uint32_t j = 0; j < itemres->size(); ++j) {
                uint32_t slot, itemid;  //, itemtype, amount;
                itemres->getInt("slot", slot);
                itemres->getInt("itemid", itemid);
                // itemres->getInt( "itemtype", itemtype );
                // itemres->getInt( "amount", amount );

                packet2->addEquipItem(slot, itemid);
                itemres->incrementRow();
              }
            }
          }

          Send(*packet2);

          auto packet3 = makePacket<ePacketType::PAKWC_INVENTORY_DATA>(zuly);
          Send(*packet3);

          auto packet4 = makePacket<ePacketType::PAKWC_QUEST_DATA>();
          Send(*packet4);

          auto packet5 = makePacket<ePacketType::PAKWC_BILLING_MESSAGE>();
          Send(*packet5);
          
          //TODO: Send other clients the AVT join packet
          //TODO: Actually make the packet structure
          //auto packet6 = makePacket<ePacketType::PAKWC_PLAYER_CHAR>(*packet2);
          //CMapServer::SendPacket(this, CMapServer::eSendType::EVERYONE_BUT_ME, *packet6);
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
  Send(*makePacket<ePacketType::PAKWC_CHANGE_MAP_REPLY>(GetId(), 81, 55, 0, 0, std::time(nullptr), 0));
  //TODO : send PAKWC_PLAYER_CHAR to EVERYONE_BUT_ME
  
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
  //TODO: Call the entity distance calc here
  return true; 
}

bool CMapClient::MouseCmdRcv(std::unique_ptr<RoseCommon::CliMouseCmd> P) {
    logger_->trace("CMapClient::MouseReply()");
    if (login_state_ != eSTATE::LOGGEDIN) {
        logger_->warn("Client {} is attempting to issue mouse commands before logging in.", GetId());
        return true;
    }
    logger_->notice("position : {}, {}", P->x(), P->y());
    // TODO : set target
    //entitySystem_->get<MovementSystem>.move(entity_, P->x(), P->y());
    CMapServer::SendPacket(this, CMapServer::eSendType::EVERYONE,
            *makePacket<ePacketType::PAKWC_MOUSE_CMD>(GetId(), P->targetId(), 0, P->x(), P->y(), P->z()));
    return true;
}

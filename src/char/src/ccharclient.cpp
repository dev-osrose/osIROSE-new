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

#include "ccharserver.h"
#include "ccharisc.h"
#include "ccharclient.h"
#include "epackettype.h"
#include "database.h"

using namespace RoseCommon;

CCharClient::CCharClient()
    : CRoseClient(),
      access_rights_(0),
      login_state_(eSTATE::DEFAULT),
      sessionId_(0),
      userid_(0),
      channelid_(0) {}

CCharClient::CCharClient(tcp::socket _sock)
    : CRoseClient(std::move(_sock)),
      access_rights_(0),
      login_state_(eSTATE::DEFAULT),
      sessionId_(0),
      userid_(0),
      channelid_(0) {}

bool CCharClient::HandlePacket(uint8_t* _buffer) {
  switch (CRosePacket::type(_buffer)) {
    case ePacketType::PAKCS_JOIN_SERVER_REQ:
      return JoinServerReply(getPacket<ePacketType::PAKCS_JOIN_SERVER_REQ>(
          _buffer));  // Allow client to connect
    case ePacketType::PAKCS_CHAR_LIST_REQ:
      return SendCharListReply();
    case ePacketType::PAKCS_CREATE_CHAR_REQ:
      return SendCharCreateReply(
          getPacket<ePacketType::PAKCS_CREATE_CHAR_REQ>(_buffer));
    case ePacketType::PAKCS_DELETE_CHAR_REQ:
      return SendCharDeleteReply(
          getPacket<ePacketType::PAKCS_DELETE_CHAR_REQ>(_buffer));
    case ePacketType::PAKCS_SELECT_CHAR_REQ:
      return SendCharSelectReply(
          getPacket<ePacketType::PAKCS_SELECT_CHAR_REQ>(_buffer));
    default:
      return CRoseClient::HandlePacket(_buffer);
  }
  return true;
}

bool CCharClient::OnReceived() { return CRoseClient::OnReceived(); }

bool CCharClient::JoinServerReply(
    std::unique_ptr<RoseCommon::CliJoinServerReq> P) {
  logger_->trace("CCharClient::JoinServerReply");

  if (login_state_ != eSTATE::DEFAULT) {
    logger_->warn("Client {} is attempting to login when already logged in.",
                  GetId());
    return true;
  }

  uint32_t sessionID = P->sessionId();
  std::string password = P->password();
  logger_->info("user {}, password {} bla", sessionID, password);

  std::unique_ptr<Core::IResult> res;
  std::string query = fmt::format("CALL get_session({}, '{}');", sessionID, password);

  Core::IDatabase& database = Core::databasePool.getDatabase();
  res = database.QStore(query);
  if (res != nullptr) {  // Query the DB
    if (res->size() != 0) {
      login_state_ = eSTATE::LOGGEDIN;
      res->getInt("userid", userid_);
      res->getInt("channelid", channelid_);

      sessionId_ = sessionID;

      auto packet = makePacket<ePacketType::PAKSC_JOIN_SERVER_REPLY>(
          SrvJoinServerReply::OK, std::time(nullptr));
      Send(*packet);
    } else {
      auto packet = makePacket<ePacketType::PAKSC_JOIN_SERVER_REPLY>(
          SrvJoinServerReply::INVALID_PASSWORD, 0);
      Send(*packet);
    }
  } else {
    auto packet = makePacket<ePacketType::PAKSC_JOIN_SERVER_REPLY>(
        SrvJoinServerReply::FAILED, 0);
    Send(*packet);
  }
  return true;
}

bool CCharClient::SendCharListReply() {
  logger_->trace("CharListReply\n");

  if (login_state_ != eSTATE::LOGGEDIN) {
    logger_->warn(
        "Client {} is attempting to get the char list before logging in.",
        GetId());
    return true;
  }

  // mysql query to get the characters created.
  std::unique_ptr<Core::IResult> res, itemres;
  std::string query = fmt::format("CALL get_character_list({});", userid_);

  Core::IDatabase& database = Core::databasePool.getDatabase();
  res = database.QStore(query);

  auto packet = makePacket<ePacketType::PAKCC_CHAR_LIST_REPLY>();
  if (res != nullptr) {
    if (res->size() != 0) {
      for (uint32_t idx = 0; idx < res->size(); ++idx) {
        std::string _name;
        uint32_t race, level, job, delete_time, face, hair, id;
        res->getString("name", _name);
        res->getInt("race", race);
        res->getInt("level", level);
        res->getInt("job", job);
        res->getInt("delete_date", delete_time);
        res->getInt("face", face);
        res->getInt("hair", hair);

        packet->addCharacter(_name, race, level, job, delete_time);
        packet->addEquipItem(
            idx, SrvCharacterListReply::equipped_position::EQUIP_FACE, face);
        packet->addEquipItem(
            idx, SrvCharacterListReply::equipped_position::EQUIP_HAIR, hair);

        {
          res->getInt("id", id);
          character_real_id_.push_back(id);
          query = fmt::format("CALL get_equipped({});", id);
          itemres = database.QStore(query);
          if (itemres != nullptr) {
            if (itemres->size() != 0) {
              for (uint32_t j = 0; j < itemres->size(); ++j) {
                uint32_t slot, itemid;
                itemres->getInt("slot", slot);
                itemres->getInt("itemid", itemid);

                packet->addEquipItem(
                    idx, (SrvCharacterListReply::equipped_position)slot,
                    itemid);
                itemres->incrementRow();
              }
            }
          }
        }

        res->incrementRow();
      }
    }
  }
  Send(*packet);

  return true;
}

bool CCharClient::SendCharCreateReply(
    std::unique_ptr<RoseCommon::CliCreateCharReq> P) {
  logger_->trace("CharCreateReply\n");

  if (login_state_ != eSTATE::LOGGEDIN) {
    logger_->warn(
        "Client {} is attempting to get the create a char before logging in.",
        GetId());
    return true;
  }
  std::string query =
      fmt::format("CALL create_char('{}', {}, {}, {}, {}, {});",
                  Core::CMySQL_Database::escapeData(P->name().c_str()), userid_,
                  P->race(), P->face(), P->hair(), P->stone());

  Core::IDatabase& database = Core::databasePool.getDatabase();
  auto res = SrvCreateCharReply::OK;
  try {
      database.QExecute(query);
  } catch (const mysqlpp::BadQuery &e) {
      res = SrvCreateCharReply::NAME_TAKEN;
  }

  auto packet = makePacket<ePacketType::PAKCC_CREATE_CHAR_REPLY>(
      res);
  Send(*packet);

  return true;
}

bool CCharClient::SendCharDeleteReply(
    std::unique_ptr<RoseCommon::CliDeleteCharReq> P) {
  logger_->trace("CharDeleteReply\n");

  if (login_state_ != eSTATE::LOGGEDIN) {
    logger_->warn("Client {} is attempting to delete a char before logging in.",
                  GetId());
    return true;
  }

  if (P->charId() > 6) return false;

  uint32_t time = 0;
  if (P->isDelete()) {
    // we need to delete the char
    time = std::time(nullptr);

    std::string query =
        fmt::format("CALL delete_character({}, '{}');", userid_,
                    Core::CMySQL_Database::escapeData(P->name().c_str()));

    Core::IDatabase& database = Core::databasePool.getDatabase();
    database.QExecute(query);
  }

  auto packet =
      makePacket<ePacketType::PAKCC_DELETE_CHAR_REPLY>(time, P->name());
  Send(*packet);
  return true;
}

bool CCharClient::SendCharSelectReply(
    std::unique_ptr<RoseCommon::CliSelectCharReq> P) {
  logger_->trace("CharSelectReply\n");

  if (login_state_ != eSTATE::LOGGEDIN) {
    logger_->warn("Client {} is attempting to select a char before logging in.",
                  GetId());
    return true;
  }

  login_state_ = eSTATE::TRANSFERING;

  std::string query = fmt::format("CALL update_session_with_character({}, '{}');",
                                  sessionId_, character_real_id_[P->charId()]);

  Core::IDatabase& database = Core::databasePool.getDatabase();
  database.QExecute(query);

  std::lock_guard<std::mutex> lock(CCharServer::GetISCListMutex());
  for (auto& server : CCharServer::GetISCList()) {
    if (server->GetType() == iscPacket::ServerType::MAP_MASTER &&
        server->GetId() == channelid_) {
      auto packet = makePacket<ePacketType::PAKCC_SWITCH_SERVER>(
          server->GetPort(), sessionId_,
          0, server->GetIpAddress());  // this should be set to the map server's encryption seed
      Send(*packet);
    }
  }

  return true;
}

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

#include "ccharclient.h"
#include "ccharisc.h"
#include "ccharserver.h"
#include "cli_createcharreq.h"
#include "cli_deletecharreq.h"
#include "cli_joinserverreq.h"
#include "cli_selectcharreq.h"
#include "connection.h"
#include "connectionpool.h"
#include "srv_channellistreply.h"
#include "srv_characterlistreply.h"
#include "srv_createcharreply.h"
#include "srv_deletecharreply.h"
#include "srv_joinserverreply.h"
#include "srv_switchserver.h"

using namespace RoseCommon;
const auto now = ::sqlpp::chrono::floor<::std::chrono::seconds>(std::chrono::system_clock::now());

CCharClient::CCharClient()
    : CRoseClient(), accessRights_(0), loginState_(eSTATE::DEFAULT), sessionId_(0), userId_(0), channelId_(0), server_(nullptr) {}

CCharClient::CCharClient(CCharServer *server, std::unique_ptr<Core::INetwork> _sock)
    : CRoseClient(std::move(_sock)),
      accessRights_(0),
      loginState_(eSTATE::DEFAULT),
      sessionId_(0),
      userId_(0),
      channelId_(0),
      server_(server) {}

bool CCharClient::HandlePacket(uint8_t *_buffer) {
  switch (CRosePacket::type(_buffer)) {
    case ePacketType::PAKCS_JOIN_SERVER_REQ:
      return JoinServerReply(getPacket<ePacketType::PAKCS_JOIN_SERVER_REQ>(_buffer));  // Allow client to connect
    case ePacketType::PAKCS_CHAR_LIST_REQ:
      return SendCharListReply();
    case ePacketType::PAKCS_CREATE_CHAR_REQ:
      return SendCharCreateReply(getPacket<ePacketType::PAKCS_CREATE_CHAR_REQ>(_buffer));
    case ePacketType::PAKCS_DELETE_CHAR_REQ:
      return SendCharDeleteReply(getPacket<ePacketType::PAKCS_DELETE_CHAR_REQ>(_buffer));
    case ePacketType::PAKCS_SELECT_CHAR_REQ:
      return SendCharSelectReply(getPacket<ePacketType::PAKCS_SELECT_CHAR_REQ>(_buffer));
    case ePacketType::PAKCS_ALIVE:
      if (loginState_ != eSTATE::LOGGEDIN) {
        logger_->warn("Client {} is attempting to execute an action before loggin in.", userId_);
        return true;
      }
      updateSession();
      CRoseClient::HandlePacket(_buffer);
      break;
    default:
      CRoseClient::HandlePacket(_buffer);
  }
  return true;
}

void CCharClient::updateSession() {
  using namespace std::chrono_literals;
  static std::chrono::steady_clock::time_point time{};
  if (Core::Time::GetTickCount() - time < 2min) return;
  time = Core::Time::GetTickCount();
  logger_->trace("CCharClient::updateSession()");
  Core::SessionTable session{};
  auto conn = Core::connectionPool.getConnection(Core::osirose);
  conn(sqlpp::update(session).set(session.time = std::chrono::system_clock::now()).where(session.userid == userId_));
}

bool CCharClient::JoinServerReply(std::unique_ptr<RoseCommon::CliJoinServerReq> P) {
  logger_->trace("CCharClient::JoinServerReply");

  if (loginState_ != eSTATE::DEFAULT) {
    logger_->warn("Client {} is attempting to login when already logged in.", get_id());
    return true;
  }

  uint32_t sessionID = P->sessionId();
  std::string password = Core::escapeData(P->password());

  auto conn = Core::connectionPool.getConnection(Core::osirose);
  Core::AccountTable accounts{};
  Core::SessionTable sessions{};
  try {
    const auto res = conn(
        sqlpp::select(sessions.userid, sessions.channelid)
            .from(sessions.join(accounts).on(sessions.userid == accounts.id))
            .where(sessions.id == sessionID and accounts.password == sqlpp::verbatim<sqlpp::varchar>(fmt::format(
                                                                         "SHA2(CONCAT('{}', salt), 256)", password))));
    if (!res.empty()) {
      loginState_ = eSTATE::LOGGEDIN;
      const auto &row = res.front();
      userId_ = row.userid;
      channelId_ = row.channelid;

      sessionId_ = sessionID;

      auto packet = makePacket<ePacketType::PAKSC_JOIN_SERVER_REPLY>(SrvJoinServerReply::OK, std::time(nullptr));
      send(*packet);
    } else {
      auto packet = makePacket<ePacketType::PAKSC_JOIN_SERVER_REPLY>(SrvJoinServerReply::INVALID_PASSWORD, 0);
      send(*packet);
    }
  } catch (const sqlpp::exception &e) {
    logger_->error("Error while accessing the database: {}", e.what());
    auto packet = makePacket<ePacketType::PAKSC_JOIN_SERVER_REPLY>(SrvJoinServerReply::FAILED, 0);
    send(*packet);
  }
  return true;
}

bool CCharClient::SendCharListReply() {
  logger_->trace("CharListReply\n");

  if (loginState_ != eSTATE::LOGGEDIN) {
    logger_->warn("Client {} is attempting to get the char list before logging in.", get_id());
    return true;
  }

  auto conn = Core::connectionPool.getConnection(Core::osirose);
  Core::CharacterTable table{};

  auto packet = makePacket<ePacketType::PAKCC_CHAR_LIST_REPLY>();
  uint32_t id = 0;

  std::time_t now_c = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  characterRealId_.clear();
  for (const auto &row : conn(sqlpp::select(sqlpp::all_of(table)).from(table).where(table.userid == userId_))) {
    auto _remaining_time = 0;  // Get time in seconds until delete
    
    if(row.deleteDate.is_null() == false)
      _remaining_time = std::difftime(std::chrono::system_clock::to_time_t(row.deleteDate.value()), now_c);
    
    packet->addCharacter(row.name, row.race, row.level, row.job, row.face, row.hair, _remaining_time);
    characterRealId_.push_back(row.id);
    Core::InventoryTable inv{};
    for (const auto &iv :
         conn(sqlpp::select(inv.slot, inv.itemid).from(inv).where(inv.charId == row.id and inv.slot < 10)))
      packet->addEquipItem(id, SrvCharacterListReply::getPosition(iv.slot), iv.itemid);
    ++id;
  }
  send(*packet);

  return true;
}

bool CCharClient::SendCharCreateReply(std::unique_ptr<RoseCommon::CliCreateCharReq> P) {
  logger_->trace("CharCreateReply\n");

  if (loginState_ != eSTATE::LOGGEDIN) {
    logger_->warn("Client {} is attempting to get the create a char before logging in.", get_id());
    return true;
  }
  std::string query = fmt::format("CALL create_char('{}', {}, {}, {}, {}, {});", Core::escapeData(P->name().c_str()),
                                  userId_, P->race(), P->face(), P->hair(), P->stone());

  auto conn = Core::connectionPool.getConnection(Core::osirose);
  auto res = SrvCreateCharReply::OK;
  try {
    conn->execute(query);
  } catch (sqlpp::exception &) {
    res = SrvCreateCharReply::NAME_TAKEN;
  }

  auto packet = makePacket<ePacketType::PAKCC_CREATE_CHAR_REPLY>(res);
  send(*packet);

  return true;
}

bool CCharClient::SendCharDeleteReply(std::unique_ptr<RoseCommon::CliDeleteCharReq> P) {
  logger_->trace("CharDeleteReply\n");

  if (loginState_ != eSTATE::LOGGEDIN) {
    logger_->warn("Client {} is attempting to delete a char before logging in.", get_id());
    return true;
  }

  //TODO: change this to be varible
  if (P->charId() > 6) return false;

  uint8_t delete_type = (uint8_t)P->isDelete();
  uint32_t time = 0;
  if (P->isDelete()) {
    //TODO: if the character is a guild leader, time = -1 and don't delete character
    // we need to delete the char
    Core::Config& config = Core::Config::getInstance();
    if(config.charServer().instantCharDelete == false) {
      //TODO: allow the server owner to set the time. This will also require the ability to change the time in sql
      time = 60*60*24; // The default is one day from now
    } else {
      time = 0;
      delete_type = 2;
    }
  }
  
  std::string query = fmt::format("CALL delete_character({}, '{}', {});", userId_, Core::escapeData(P->name().c_str()), delete_type);

  auto conn = Core::connectionPool.getConnection(Core::osirose);
  conn->execute(query);

  // if time == -1, delete failed
  auto packet = makePacket<ePacketType::PAKCC_DELETE_CHAR_REPLY>(time, P->name());
  send(*packet);
  return true;
}

void CCharClient::OnDisconnected() {
  logger_->trace("CCharClient::OnDisconnected()");
  Core::SessionTable session{};
  Core::AccountTable table{};
  auto conn = Core::connectionPool.getConnection(Core::osirose);
  const auto res = conn(sqlpp::select(table.online).from(table).where(table.id == userId_));
  if (!res.empty())
    if (res.front().online) conn(sqlpp::remove_from(session).where(session.userid == userId_));
  conn(sqlpp::update(table).set(table.online = 0).where(table.id == userId_));
}

bool CCharClient::SendCharSelectReply(std::unique_ptr<RoseCommon::CliSelectCharReq> P) {
  logger_->trace("CharSelectReply\n");

  if (loginState_ != eSTATE::LOGGEDIN) {
    logger_->warn("Client {} is attempting to select a char before logging in.", get_id());
    return true;
  }

  uint8_t selected_id = P->charId();
  if (selected_id > characterRealId_.size()) {
    logger_->warn("Client {} is attempting to select a invalid character.", get_id());
    return false;
  }
  // if (characterRealId_[selected_id]) {
  //   logger_->warn("Client {} is attempting to select a character that is being deleted.", get_id());
  //   return false;
  // }
  
  loginState_ = eSTATE::TRANSFERING;

  std::string query =
      fmt::format("CALL update_session_with_character({}, '{}');", sessionId_, characterRealId_[selected_id]);

  auto conn = Core::connectionPool.getConnection(Core::osirose);
  conn->execute(query);

  Core::CharacterTable table{};
  auto charRes = conn(sqlpp::select(table.map).from(table).where(table.id == characterRealId_[selected_id]));
  uint16_t map_id = charRes.front().map;

  std::lock_guard<std::mutex> lock(server_->GetISCListMutex());
  for (auto &server : server_->GetISCList()) {
    if (server->get_type() == Isc::ServerType::MAP_MASTER && server->get_id() == channelId_) {
      auto packet = makePacket<ePacketType::PAKCC_SWITCH_SERVER>(
          server->get_port() + map_id, sessionId_, 0,
          server->get_address());  // this should be set to the map server's encryption seed
      send(*packet);
    }
  }

  return true;
}

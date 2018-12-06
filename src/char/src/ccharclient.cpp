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
#include "connection.h"
#include "connectionpool.h"
#include "srv_channel_list_reply.h"
#include "srv_char_list_reply.h"
#include "srv_create_char_reply.h"
#include "srv_delete_char_reply.h"
#include "srv_join_server_reply.h"
#include "srv_switch_server.h"

using namespace RoseCommon;

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

bool CCharClient::handlePacket(uint8_t *_buffer) {
  switch (CRosePacket::type(_buffer)) {
    case ePacketType::PAKCS_JOIN_SERVER_REQ:
      return joinServerReply(Packet::CliJoinServerReq::create(_buffer));  // Allow client to connect
    case ePacketType::PAKCS_CHAR_LIST_REQ:
      return sendCharListReply();
    case ePacketType::PAKCS_CREATE_CHAR_REQ:
      return sendCharCreateReply(Packet::CliCreateCharReq::create(_buffer));
    case ePacketType::PAKCS_DELETE_CHAR_REQ:
      return sendCharDeleteReply(Packet::CliDeleteCharReq::create(_buffer));
    case ePacketType::PAKCS_SELECT_CHAR_REQ:
      return sendCharSelectReply(Packet::CliSelectCharReq::create(_buffer));
    case ePacketType::PAKCS_ALIVE:
      if (loginState_ != eSTATE::LOGGEDIN) {
        logger_->warn("Client {} is attempting to execute an action before loggin in.", userId_);
        return true;
      }
      updateSession();
      [[fallthrough]];
    default:
      CRoseClient::handlePacket(_buffer);
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

bool CCharClient::joinServerReply(RoseCommon::Packet::CliJoinServerReq&& P) {
  logger_->trace("CCharClient::joinServerReply");

  if (loginState_ != eSTATE::DEFAULT) {
    logger_->warn("Client {} is attempting to login when already logged in.", get_id());
    return true;
  }

  uint32_t sessionID = P.get_sessionId();
  std::string password = Core::escapeData(P.get_password());

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
      isLoggedIn_ = true;
      const auto &row = res.front();
      userId_ = row.userid;
      channelId_ = row.channelid;

      sessionId_ = sessionID;

      auto packet = Packet::SrvJoinServerReply::create(Packet::SrvJoinServerReply::OK, std::time(nullptr));
      send(packet);
    } else {
      auto packet = Packet::SrvJoinServerReply::create(Packet::SrvJoinServerReply::INVALID_PASSWORD, 0);
      send(packet);
    }
  } catch (const sqlpp::exception &e) {
    logger_->error("Error while accessing the database: {}", e.what());
    auto packet = Packet::SrvJoinServerReply::create(Packet::SrvJoinServerReply::FAILED, 0);
    send(packet);
  }
  return true;
}

bool CCharClient::sendCharListReply() {
  logger_->trace("CCharClient::sendCharListReply");

  if (loginState_ != eSTATE::LOGGEDIN) {
    logger_->warn("Client {} is attempting to get the char list before logging in.", get_id());
    return true;
  }

  auto conn = Core::connectionPool.getConnection(Core::osirose);
  Core::CharacterTable table{};

  auto packet = Packet::SrvCharListReply::create();

  characterRealId_.clear();
  for (const auto &row : conn(sqlpp::select(sqlpp::all_of(table)).from(table).where(table.userid == userId_))) {
      Packet::SrvCharListReply::CharInfo charInfo;
      charInfo.set_name(row.name);
      charInfo.set_race(row.race);
      charInfo.set_level(row.level);
      charInfo.set_job(row.job);
      charInfo.set_face(row.face);
      charInfo.set_hair(row.hair);
      charInfo.set_remainSecsUntilDelete(row.deleteDate);
    characterRealId_.push_back(row.id);
    Core::InventoryTable inv{};
    for (const auto &iv : conn(sqlpp::select(inv.slot, inv.itemid).from(inv).where(inv.charId == row.id and inv.slot < 10))) {
        Packet::SrvCharListReply::EquippedItem item;
        item.set_id(iv.itemid);
        item.set_gem_opt(0);
        item.set_socket(0);
        item.set_grade(0);
        charInfo.set_items(item, iv.slot);
    }
    packet.add_characters(charInfo);
  }
  send(packet);

  return true;
}

bool CCharClient::sendCharCreateReply(RoseCommon::Packet::CliCreateCharReq&& P) {
  logger_->trace("CCharClient::sendCharCreateReply");

  if (loginState_ != eSTATE::LOGGEDIN) {
    logger_->warn("Client {} is attempting to get the create a char before logging in.", get_id());
    return true;
  }
  std::string query = fmt::format("CALL create_char('{}', {}, {}, {}, {}, {});", Core::escapeData(P.get_name().c_str()),
                                  userId_, P.get_race(), P.get_face(), P.get_hair(), P.get_stone());

  auto conn = Core::connectionPool.getConnection(Core::osirose);
  auto res = Packet::SrvCreateCharReply::OK;
  try {
    conn->execute(query);
  } catch (sqlpp::exception &) {
    res = Packet::SrvCreateCharReply::NAME_TAKEN;
  }

  auto packet = Packet::SrvCreateCharReply::create(res);
  send(packet);

  return true;
}

bool CCharClient::sendCharDeleteReply(RoseCommon::Packet::CliDeleteCharReq&& P) {
  logger_->trace("CCharClient::sendCharDeleteReply");

  if (loginState_ != eSTATE::LOGGEDIN) {
    logger_->warn("Client {} is attempting to delete a char before logging in.", get_id());
    return true;
  }

  if (P.get_charId() > 6) return false;

  uint32_t time = 0;
  if (P.get_isDelete()) {
    // we need to delete the char
    time = std::time(nullptr);

    std::string query = fmt::format("CALL delete_character({}, '{}');", userId_, Core::escapeData(P.get_name().c_str()));

    auto conn = Core::connectionPool.getConnection(Core::osirose);
    conn->execute(query);
  }

  auto packet = Packet::SrvDeleteCharReply::create(time, P.get_name());
  send(packet);
  return true;
}

void CCharClient::onDisconnected() {
  logger_->trace("CCharClient::onDisconnected()");
  Core::SessionTable session{};
  Core::AccountTable table{};
  auto conn = Core::connectionPool.getConnection(Core::osirose);
  const auto res = conn(sqlpp::select(table.online).from(table).where(table.id == userId_));
  if (!res.empty())
    if (res.front().online) conn(sqlpp::remove_from(session).where(session.userid == userId_));
  conn(sqlpp::update(table).set(table.online = 0).where(table.id == userId_));
}

bool CCharClient::sendCharSelectReply(RoseCommon::Packet::CliSelectCharReq&& P) {
  logger_->trace("CCharClient::sendCharSelectReply");

  if (loginState_ != eSTATE::LOGGEDIN) {
    logger_->warn("Client {} is attempting to select a char before logging in.", get_id());
    return true;
  }

  loginState_ = eSTATE::TRANSFERING;

  uint8_t selected_id = P.get_charId();
  if (selected_id > characterRealId_.size()) {
    logger_->warn("Client {} is attempting to select a invalid character.", get_id());
    return false;
  }

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
      auto packet = Packet::SrvSwitchServer::create(
          server->get_port() + map_id, sessionId_, 0,
          server->get_address());  // this should be set to the map server's encryption seed
      send(packet);
    }
  }

  return true;
}

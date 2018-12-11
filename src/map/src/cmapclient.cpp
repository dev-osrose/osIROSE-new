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

#include <cmath>
#include "cmapclient.h"
#include "cmapisc.h"
#include "cmapserver.h"
#include "config.h"
#include "connection.h"
#include "epackettype.h"
#include "srv_inventory_data.h"
#include "srv_join_server_reply.h"
#include "srv_quest_data.h"
#include "srv_select_char_reply.h"
//#include "srv_teleport_reply.h"
#include "srv_billing_message.h"

#include "components/basic_info.h"

using namespace RoseCommon;

CMapClient::CMapClient()
    : CRoseClient(), access_rights_(0), login_state_(eSTATE::DEFAULT), sessionId_(0), userid_(0), charid_(0) {}

CMapClient::CMapClient(std::unique_ptr<Core::INetwork> _sock, std::shared_ptr<EntitySystem> entitySystem)
    : CRoseClient(std::move(_sock)),
      access_rights_(0),
      login_state_(eSTATE::DEFAULT),
      sessionId_(0),
      userid_(0),
      charid_(0),
      entitySystem(entitySystem) {}

CMapClient::~CMapClient() {}

bool CMapClient::handlePacket(uint8_t* _buffer) {
  switch (CRosePacket::type(_buffer)) {
    case ePacketType::PAKCS_ALIVE:
      if (login_state_ != eSTATE::LOGGEDIN) {
        // logger_->warn("Client {} is attempting to execute an action before logging in.", get_id());
        return true;
      }
      updateSession();
      break;
    case ePacketType::PAKCS_JOIN_SERVER_REQ:
      return joinServerReply(Packet::CliJoinServerReq::create(_buffer));  // Allow client to connect
    case ePacketType::PAKCS_CHANGE_CHAR_REQ: {
      logger_->warn("Change character hasn't been implemented yet.");
      // TODO: Send ePacketType::PAKCC_CHAN_CHAR_REPLY to the client with the character/node server ip and port to
      // change their character
      return true;
    }
    case ePacketType::PAKCS_CHANGE_MAP_REQ:
      if (login_state_ != eSTATE::LOGGEDIN) {
        logger_->warn("Client {} is attempting to execute an action before logging in.", get_id());
        return true;
      }
      break;
    case ePacketType::PAKCS_ACCEPT_REQ:
      return CRoseClient::handlePacket(_buffer);
    default:
      break;
  }
  if (login_state_ != eSTATE::LOGGEDIN) {
    logger_->warn("Client {} is attempting to execute an action before logging in.", get_id());
    return CRoseClient::handlePacket(_buffer);
  }
  logger_->warn("Packet {} not handled", to_underlying(CRosePacket::type(_buffer)));
  return true;
}

void CMapClient::updateSession() {
  logger_->trace("CMapClient::updateSession()");
  using namespace std::chrono_literals;
  static std::chrono::steady_clock::time_point time{};

  if (Core::Time::GetTickCount() - time < 2min)
    return;

  time = Core::Time::GetTickCount();
  Core::SessionTable session{};
  auto conn = Core::connectionPool.getConnection(Core::osirose);
  conn(sqlpp::update(session).set(session.time = std::chrono::system_clock::now()).where(session.userid == get_id()));
}

void CMapClient::onDisconnected() {
  logger_->trace("CMapClient::OnDisconnected()");
  if (login_state_ == eSTATE::DEFAULT) return;

  if (login_state_ != eSTATE::SWITCHING) {
      Core::AccountTable table{};
      auto conn = Core::connectionPool.getConnection(Core::osirose);
      conn(sqlpp::update(table).set(table.online = 0).where(table.id == get_id()));
  }
  login_state_ = eSTATE::DEFAULT;
}

bool CMapClient::joinServerReply(RoseCommon::Packet::CliJoinServerReq&& P) {
  logger_->trace("CMapClient::joinServerReply()");

  if (login_state_ != eSTATE::DEFAULT) {
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
        sqlpp::select(sessions.userid, sessions.charid, sessions.worldip, accounts.platinium, accounts.access)
            .from(sessions.join(accounts).on(sessions.userid == accounts.id))
            .where(sessions.id == sessionID and accounts.password == sqlpp::verbatim<sqlpp::varchar>(fmt::format(
                                                                         "SHA2(CONCAT('{}', salt), 256)", password))));

    if (!res.empty()) {
      logger_->debug("Client {} auth OK.", get_id());
      login_state_ = eSTATE::LOGGEDIN;
      isLoggedIn_ = true;
      const auto& row = res.front();
      userid_ = row.userid;
      charid_ = row.charid;
      sessionId_ = sessionID;
      bool platinium = false;
      platinium = row.platinium;

      auto entity = entitySystem->load_character(charid_, platinium);

      if (true) {
        Core::Config& config = Core::Config::getInstance();
        conn(sqlpp::update(sessions)
                 .set(sessions.worldip = config.serverData().ip, sessions.worldport = config.mapServer().clientPort)
                 .where(sessions.id == sessionID));

        send(Packet::SrvJoinServerReply::create(Packet::SrvJoinServerReply::OK, 0)); // TODO: replace with a normal ID from the ECS

        if (row.worldip.is_null()) { // if there is already a world ip, the client is switching servers so we shouldn't send it the starting data
          // SEND PLAYER DATA HERE!!!!!!
          const auto& basicInfo = entitySystem->get<Component::BasicInfo>(entity);
          send(Packet::SrvSelectCharReply::create());

          send(Packet::SrvInventoryData::create((int64_t)0)); // it's zuly

          send(Packet::SrvQuestData::create());

          send(Packet::SrvBillingMessage::create());
        } else {
          //send(Packet::SrvTeleportReply::create(entity_));
        }

      } else {
        logger_->debug("Something wrong happened when creating the entity");
        send(Packet::SrvJoinServerReply::create(Packet::SrvJoinServerReply::FAILED, 0));
      }
    } else {
      logger_->debug("Client {} auth INVALID_PASS.", get_id());
      send(Packet::SrvJoinServerReply::create(Packet::SrvJoinServerReply::INVALID_PASSWORD, 0));
    }
  } catch (const sqlpp::exception& e) {
    logger_->error("Error while accessing the database: {}", e.what());
    send(Packet::SrvJoinServerReply::create(Packet::SrvJoinServerReply::FAILED, 0));
  }
  return true;
}

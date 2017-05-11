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
#include "entitycomponents.h"
#include <cmath>
#include "systems/chatsystem.h"

using namespace RoseCommon;

CMapClient::CMapClient()
    : CRoseClient(),
      access_rights_(0),
      login_state_(eSTATE::DEFAULT),
      sessionId_(0),
      userid_(0),
      charid_(0),
      canBeDeleted_(true) {}

CMapClient::CMapClient(std::unique_ptr<Core::INetwork> _sock, std::shared_ptr<EntitySystem> entitySystem)
    : CRoseClient(std::move(_sock)),
      access_rights_(0),
      login_state_(eSTATE::DEFAULT),
      sessionId_(0),
      userid_(0),
      charid_(0),
      entitySystem_(entitySystem),
      canBeDeleted_(true)
      {}

bool CMapClient::HandlePacket(uint8_t* _buffer) {
  switch (CRosePacket::type(_buffer)) {
    case ePacketType::PAKCS_JOIN_SERVER_REQ:
      return JoinServerReply(getPacket<ePacketType::PAKCS_JOIN_SERVER_REQ>(
          _buffer));  // Allow client to connect
    case ePacketType::PAKCS_CHANGE_MAP_REQ:
            entity_.component<BasicInfo>()->loggedIn_.store(false);
        default:
            break;
    }
        auto packet = fetchPacket(_buffer);
    if (!packet) {
        logger_->warn("Couldn't build the packet");
            return CRoseClient::HandlePacket(_buffer);
    }

        if (login_state_ != eSTATE::LOGGEDIN) {
          logger_->warn("Client {} is attempting to execute an action before logging in.",
                        get_id());
          return true;
        }
    if (!entitySystem_->dispatch(entity_, std::move(packet))) {
        logger_->warn("There is no system willing to deal with this packet");
            CRoseClient::HandlePacket(_buffer); // FIXME : removed the return because I want to be able to mess around with unkown packets for the time being
    }
  return true;
}

void CMapClient::OnDisconnected() {
    entitySystem_->saveCharacter(charid_, entity_);
    CMapServer::SendPacket(this, CMapServer::eSendType::EVERYONE_BUT_ME, *makePacket<ePacketType::PAKWC_REMOVE_OBJECT>(entity_));
    entitySystem_->destroy(entity_);
}

bool CMapClient::OnShutdown() {
    set_update_time(std::chrono::steady_clock::time_point());
    if (!canBeDeleted_.load())
        return false;
    return true;
}

bool CMapClient::JoinServerReply(
    std::unique_ptr<RoseCommon::CliJoinServerReq> P) {
  logger_->trace("CMapClient::JoinServerReply()");

  if (login_state_ != eSTATE::DEFAULT) {
    logger_->warn("Client {} is attempting to login when already logged in.",
                  get_id());
    return true;
  }

  uint32_t sessionID = P->sessionId();
  std::string password = Core::CMySQL_Database::escapeData(P->password());

  std::unique_ptr<Core::IResult> res, itemres;
  std::string query = fmt::format("CALL get_session({}, '{}');", sessionID, password);

  Core::IDatabase& database = Core::databasePool.getDatabase();
  res = database.QStore(query);
  if (res != nullptr) {  // Query the DB
    if (res->size() != 0) {
      logger_->debug("Client {} auth OK.", get_id());
      login_state_ = eSTATE::LOGGEDIN;
      res->getInt("userid", userid_);
      res->getInt("charid", charid_);
      sessionId_ = sessionID;
      bool platinium = false;
      res->getInt("platinium", platinium);
      entity_ = entitySystem_->loadCharacter(charid_, platinium, get_id());

      if (entity_) {
        canBeDeleted_.store(false);
        entity_.assign<SocketConnector>(this);

        auto packet = makePacket<ePacketType::PAKSC_JOIN_SERVER_REPLY>(
            SrvJoinServerReply::OK, std::time(nullptr));
        send(*packet);

        // SEND PLAYER DATA HERE!!!!!!
        auto packet2 = makePacket<ePacketType::PAKWC_SELECT_CHAR_REPLY>(entity_);
        send(*packet2);

        auto packet3 = makePacket<ePacketType::PAKWC_INVENTORY_DATA>(entity_);
        send(*packet3);

        auto packet4 = makePacket<ePacketType::PAKWC_QUEST_DATA>();
        send(*packet4);

        auto packet5 = makePacket<ePacketType::PAKWC_BILLING_MESSAGE>();
        send(*packet5);

      } else {
          logger_->debug("Something wrong happened when creating the entity");
          auto packet = makePacket<ePacketType::PAKSC_JOIN_SERVER_REPLY>(
              SrvJoinServerReply::FAILED, 0);
          send(*packet);
      }
    } else {
      logger_->debug("Client {} auth INVALID_PASS.", get_id());
      auto packet = makePacket<ePacketType::PAKSC_JOIN_SERVER_REPLY>(
          SrvJoinServerReply::INVALID_PASSWORD, 0);
      send(*packet);
    }
   } else {
      logger_->debug("Client {} auth FAILED.", get_id());
      auto packet = makePacket<ePacketType::PAKSC_JOIN_SERVER_REPLY>(
          SrvJoinServerReply::FAILED, 0);
      send(*packet);
    }
  return true;
};

bool CMapClient::is_nearby(const CRoseClient* _otherClient) const {
  logger_->trace("CMapClient::is_nearby()");
  return EntitySystem::isNearby(entity_, _otherClient->getEntity());
}

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
#include "transform.h"
#include "cmapclient.h"
#include "cmapisc.h"
#include "cmapserver.h"
#include "config.h"
#include "connection.h"
#include "epackettype.h"
#include "entity.h"
#include "srv_inventory_data.h"
#include "srv_join_server_reply.h"
#include "srv_quest_data.h"
#include "srv_select_char_reply.h"
#include "srv_billing_message.h"
#include "srv_teleport_reply.h"
#include "srv_logout_reply.h"
#include "srv_chan_char_reply.h"
#include "combat/combat.h"
#include "utils/calculation.h"

#include "components/basic_info.h"
#include "components/faction.h"
#include "components/character_graphics.h"
#include "components/computed_values.h"
#include "components/destination.h"
#include "components/guild.h"
#include "components/hotbar.h"
#include "components/inventory.h"
#include "components/level.h"
#include "components/life.h"
#include "components/magic.h"
#include "components/mob.h"
#include "components/npc.h"
#include "components/owner.h"
#include "components/position.h"
#include "components/skills.h"
#include "components/stamina.h"
#include "components/stats.h"
#include "components/status_effects.h"
#include "components/target.h"
#include "components/wishlist.h"
#include "itemdb.h"

#include "isc_client_status.h"

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
    case ePacketType::PAKCS_CHANGE_CHAR_REQ:
      return changeCharacterReply(Packet::CliChangeCharReq::create(_buffer));
    case ePacketType::PAKCS_CHANGE_MAP_REQ:
      if (login_state_ != eSTATE::LOGGEDIN && login_state_ != eSTATE::ONMAP) {
        logger_->warn("Client {} is attempting to execute an action before logging in.", get_id());
        return true;
      }
      break;
    case ePacketType::PAKCS_ACCEPT_REQ:
      return CRoseClient::handlePacket(_buffer);
    case ePacketType::PAKCS_LOGOUT_REQ:
      return logoutReply();
    default:
      break;
  }
  if (login_state_ == eSTATE::DEFAULT) {
    logger_->warn("Client {} is attempting to execute an action before logging in.", get_id());
    return CRoseClient::handlePacket(_buffer);
  }
  if (entitySystem->dispatch_packet(entity, RoseCommon::fetchPacket(_buffer))) {
      return true;
  }
  logger_->warn("Packet 0x{0:02x} not handled", to_underlying(CRosePacket::type(_buffer)));
  return true;
}

void CMapClient::set_on_map() {
    login_state_ = eSTATE::ONMAP;
}

void CMapClient::switch_server() {
  login_state_ = eSTATE::SWITCHING;
}

void CMapClient::send(const RoseCommon::CRosePacket& packet, bool force) {
    if (login_state_ == eSTATE::ONMAP || force) {
        CRoseClient::send(packet);
    }
}

void CMapClient::updateSession() {
  logger_->trace("CMapClient::updateSession()");
  using namespace std::chrono_literals;
  static std::chrono::steady_clock::time_point time{};

  if (Core::Time::GetTickCount() - time < 2min)
    return;

  time = Core::Time::GetTickCount();
  Core::SessionTable session{};
  auto conn = Core::connectionPool.getConnection<Core::Osirose>();
  conn(sqlpp::update(session).set(session.time = std::chrono::system_clock::now()).where(session.userid == get_id()));
}

void CMapClient::onDisconnected() {
  logger_->trace("CMapClient::OnDisconnected()");
  if (login_state_ == eSTATE::DEFAULT) return;
  auto tmp_state = login_state_;
  login_state_ = eSTATE::DEFAULT;
  const auto& basic = entitySystem->get_component<Component::BasicInfo>(entity);
  const auto charId = basic.charId;
  const auto entityId = basic.id;
  entitySystem->save_character(entity);
  entitySystem->delete_entity(entity);

  if (tmp_state != eSTATE::SWITCHING) {
      entitySystem->send_to_char_server(
          RoseCommon::Packet::IscClientStatus::create(entityId, RoseCommon::Packet::IscClientStatus::DISCONNECTED), charId);
      Core::AccountTable table{};
      auto conn = Core::connectionPool.getConnection<Core::Osirose>();
      conn(sqlpp::update(table).set(table.online = 0).where(table.id == get_id()));
  }
}

bool CMapClient::changeCharacterReply([[maybe_unused]] RoseCommon::Packet::CliChangeCharReq&& P) {
  logger_->trace("CMapClient::changeCharacterReply()");
  login_state_ = eSTATE::SWITCHING;
  auto conn = Core::connectionPool.getConnection<Core::Osirose>();
  Core::SessionTable sessions{};
  conn(sqlpp::update(sessions).set(sessions.worldip = sqlpp::tvin(""), sessions.worldport = sqlpp::tvin(0)).where(sessions.id == sessionId_));
  CRoseClient::send(Packet::SrvChanCharReply::create());
  return true;
}

bool CMapClient::logoutReply() {
  logger_->trace("CMapClient::logoutReply()");
  uint16_t waitTime = 0;
  CRoseClient::send(Packet::SrvLogoutReply::create(waitTime));
  return true;
}

bool CMapClient::joinServerReply(RoseCommon::Packet::CliJoinServerReq&& P) {
  logger_->trace("CMapClient::joinServerReply()");

  if (login_state_ != eSTATE::DEFAULT) {
    logger_->warn("Client {} is attempting to login when already logged in.", get_id());
    return true;
  }

  uint32_t sessionID = P.get_sessionId();
  std::string password = Core::escapeData(P.get_password());

  auto conn = Core::connectionPool.getConnection<Core::Osirose>();
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
      const auto& row = res.front();
      userid_ = row.userid;
      charid_ = row.charid;
      sessionId_ = sessionID;

      auto entity_future = entitySystem->load_character(charid_, row.access, sessionID, weak_from_this());
      if (!entity_future.valid()) {
          logger_->error("Error, future on character loading is not valid!");
          CRoseClient::send(Packet::SrvJoinServerReply::create(Packet::SrvJoinServerReply::FAILED, 0));
          return true;
      }
      entity_future.wait();
      entity = entity_future.get();
      logger_->debug("character loaded");

      if (entity != entt::null) {
        Core::Config& config = Core::Config::getInstance();
        conn(sqlpp::update(sessions)
                 .set(sessions.worldip = config.serverData().externalIp, sessions.worldport = config.mapServer().clientPort)
                 .where(sessions.id == sessionID));

        const auto& basicInfo = entitySystem->get_component<Component::BasicInfo>(entity);
        logger_->debug("Entity {} has name {}", entity, basicInfo.name);
        set_name(basicInfo.name);

        CRoseClient::send(Packet::SrvJoinServerReply::create(Packet::SrvJoinServerReply::OK, basicInfo.id));

        if (row.worldip.is_null()) { // if there is already a world ip, the client is switching servers so we shouldn't send it the starting data
          // SEND PLAYER DATA HERE!!!!!!

          auto packet = Packet::SrvSelectCharReply::create();
          const auto& characterGraphics = entitySystem->get_component<Component::CharacterGraphics>(entity);
          auto& position = entitySystem->get_component<Component::Position>(entity);
          const auto& inventory = entitySystem->get_component<Component::Inventory>(entity);
          const auto& faction = entitySystem->get_component<Component::Faction>(entity);
          const auto& guild = entitySystem->get_component<Component::Guild>(entity);
          const auto& level = entitySystem->get_component<Component::Level>(entity);
          const auto& stats = entitySystem->get_component<Component::Stats>(entity);
          const auto& life = entitySystem->get_component<Component::Life>(entity);
          const auto& magic = entitySystem->get_component<Component::Magic>(entity);
          const auto& stamina = entitySystem->get_component<Component::Stamina>(entity);
          const auto& skills = entitySystem->get_component<Component::Skills>(entity);
          const auto& hotbar = entitySystem->get_component<Component::Hotbar>(entity);
          auto& computed_values = entitySystem->get_component<Component::ComputedValues>(entity);
          computed_values.runSpeed = Calculations::get_runspeed(*entitySystem, entity);
          packet.set_race(characterGraphics.race);

          auto spawn = Combat::get_spawn_point(*entitySystem, entity, Packet::CliReviveReq::ReviveRequest::START_POSITION);
          entitySystem->update_position(entity, std::get<1>(spawn), std::get<2>(spawn));
          
          packet.set_map(position.map);
          packet.set_x(position.x);
          packet.set_y(position.y);
          packet.set_spawn(position.spawn);
          packet.set_bodyFace(characterGraphics.face);
          packet.set_bodyHair(characterGraphics.hair);
          packet.set_equippedItems(Core::transform(inventory.getVisible(), [this](const auto& en) {
              return entitySystem->item_to_equipped<decltype(packet)>(en);
          }));
          packet.set_stone(basicInfo.stone);
          packet.set_face(characterGraphics.face);
          packet.set_hair(characterGraphics.hair);
          packet.set_job(basicInfo.job);
          packet.set_factionId(faction.id);
          packet.set_factionRank(faction.rank);
          packet.set_fame(faction.fame);
          packet.set_str(stats.str);
          packet.set_dex(stats.dex);
          packet.set_int_(stats.int_);
          packet.set_con(stats.con);
          packet.set_charm(stats.charm);
          packet.set_sense(stats.sense);
          packet.set_hp(life.hp);
          packet.set_mp(magic.mp);
          packet.set_xp(level.xp);
          packet.set_level(level.level);
          packet.set_statPoints(basicInfo.statPoints);
          packet.set_skillPoints(basicInfo.skillPoints);
          packet.set_bodySize(stats.bodySize);
          packet.set_headSize(stats.headSize);
          packet.set_penaltyXp(level.penaltyXp);
          packet.set_factionFame(faction.factionFame);
          packet.set_factionPoints(faction.points);
          packet.set_guildId(guild.id);
          packet.set_guildContribution(guild.contribution);
          packet.set_guildRank(guild.rank);
          packet.set_pkFlag(basicInfo.pkFlag);
          packet.set_stamina(stamina.stamina);
          packet.set_skills(Core::transform(skills.skills, [](const auto& skill) {
              return skill.get_id();
          }));
          packet.set_hotbar(hotbar.items);
          packet.set_tag(basicInfo.tag);
          packet.set_name(basicInfo.name);

          CRoseClient::send(packet);

          auto packetInv = Packet::SrvInventoryData::create(inventory.zuly);
          packetInv.set_items(Core::transform(inventory.items, [this](const auto& entity) {
              return entitySystem->item_to_item<decltype(packetInv)>(entity);
          }));
          CRoseClient::send(packetInv);

          CRoseClient::send(Packet::SrvQuestData::create());

          CRoseClient::send(Packet::SrvBillingMessage::create());
        } else {
          const auto& basic = entitySystem->get_component<Component::BasicInfo>(entity);
          const auto& computed_values = entitySystem->get_component<Component::ComputedValues>(entity);
          const auto& pos = entitySystem->get_component<Component::Position>(entity);
          CRoseClient::send(RoseCommon::Packet::SrvTeleportReply::create(
              basic.id,
              pos.map,
              pos.x,
              pos.y,
              computed_values.moveMode,
              0 // computed_values.rideMode (FIXME: we don't have it yet)
          ));
        }
        login_state_ = eSTATE::LOGGEDIN;
      } else {
        logger_->debug("Something wrong happened when creating the entity");
        CRoseClient::send(Packet::SrvJoinServerReply::create(Packet::SrvJoinServerReply::FAILED, 0));
      }
    } else {
      logger_->debug("Client {} auth INVALID_PASS.", get_id());
      CRoseClient::send(Packet::SrvJoinServerReply::create(Packet::SrvJoinServerReply::INVALID_PASSWORD, 0));
    }
  } catch (const sqlpp::exception& e) {
    logger_->error("Error while accessing the database: {}", e.what());
    CRoseClient::send(Packet::SrvJoinServerReply::create(Packet::SrvJoinServerReply::FAILED, 0));
  }
  return true;
}

RoseCommon::Packet::SrvPlayerChar CMapClient::create_srv_player_char(const EntitySystem& entitySystem, Entity entity) {
    const auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(entity);
    const auto& characterGraphics = entitySystem.get_component<Component::CharacterGraphics>(entity);
    const auto& position = entitySystem.get_component<Component::Position>(entity);
    const auto& inventory = entitySystem.get_component<Component::Inventory>(entity);
    const auto& level = entitySystem.get_component<Component::Level>(entity);
    const auto& life = entitySystem.get_component<Component::Life>(entity);
    const auto& computedValues = entitySystem.get_component<Component::ComputedValues>(entity);

    auto packet = Packet::SrvPlayerChar();
    packet.set_id(basicInfo.id);
    packet.set_x(position.x);
    packet.set_y(position.y);
    if (const auto* dest = entitySystem.try_get_component<Component::Destination>(entity)) {
        packet.set_destX(dest->x);
        packet.set_destY(dest->y);
    } else {
        packet.set_destX(position.x);
        packet.set_destY(position.y);
    }
    packet.set_command(computedValues.command);
    if (const auto* target = entitySystem.try_get_component<Component::Target>(entity); target && target->target != entt::null) {
        const auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(target->target);
        packet.set_targetId(basicInfo.id);
    } else {
        packet.set_targetId(0);
    }
    packet.set_moveMode(computedValues.moveMode);
    packet.set_hp(life.hp);
    packet.set_teamId(basicInfo.teamId);
    packet.set_statusFlag(computedValues.statusFlag);
    packet.set_race(characterGraphics.race);
    packet.set_runSpeed(computedValues.runSpeed);
    packet.set_atkSpeed(computedValues.atkSpeed);
    packet.set_weightRate(computedValues.weightRate);
    packet.set_face(characterGraphics.face);
    packet.set_hair(characterGraphics.hair);
    packet.set_inventory(Core::transform(inventory.getVisible(), [&entitySystem](const auto& e) {
        return entitySystem.item_to_equipped<decltype(packet)>(e);
    }));
    packet.set_bullets(Core::transform(inventory.getBullets(), [&entitySystem](const auto& e) {
        return entitySystem.item_to_header<decltype(packet)>(e);
    }));
    packet.set_job(basicInfo.job);
    packet.set_level(level.level);
    packet.set_z(position.z);
    packet.set_subFlag(computedValues.subFlag);
    packet.set_name(basicInfo.name);
    packet.set_otherName(basicInfo.name);
  return packet;
}

RoseCommon::Packet::SrvNpcChar CMapClient::create_srv_npc_char(const EntitySystem& entitySystem, Entity entity) {
  const auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(entity);
  const auto& pos = entitySystem.get_component<Component::Position>(entity);
  const auto& computedValues = entitySystem.get_component<Component::ComputedValues>(entity);
  const auto& npc = entitySystem.get_component<Component::Npc>(entity);
  const auto& life = entitySystem.get_component<Component::Life>(entity);

  auto packet = Packet::SrvNpcChar::create(basicInfo.id);
  packet.set_x(pos.x);
  packet.set_y(pos.y);
  if (const auto* dest = entitySystem.try_get_component<Component::Destination>(entity)) {
    packet.set_destX(dest->x);
    packet.set_destY(dest->y);
  }
  packet.set_command(computedValues.command);
  if (const auto* target = entitySystem.try_get_component<Component::Target>(entity); target && target->target != entt::null) {
      const auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(target->target);
      packet.set_targetId(basicInfo.id);
  } else {
      packet.set_targetId(0);
  }
  packet.set_moveMode(computedValues.moveMode);
  packet.set_hp(life.hp);
  packet.set_teamId(basicInfo.teamId);
  packet.set_statusFlag(computedValues.statusFlag);
  packet.set_npcId(npc.id);
  packet.set_questId(npc.quest);
  packet.set_angle(npc.angle);
  packet.set_eventStatus(npc.event_status);

  return packet;
}

RoseCommon::Packet::SrvMobChar CMapClient::create_srv_mob_char(const EntitySystem& entitySystem, Entity entity) {
  const auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(entity);
  const auto& pos = entitySystem.get_component<Component::Position>(entity);
  const auto& computedValues = entitySystem.get_component<Component::ComputedValues>(entity);
  const auto& mob = entitySystem.get_component<Component::Mob>(entity);
  const auto& life = entitySystem.get_component<Component::Life>(entity);

  auto packet = Packet::SrvMobChar::create(basicInfo.id);
  packet.set_x(pos.x);
  packet.set_y(pos.y);
  if (const auto* dest = entitySystem.try_get_component<Component::Destination>(entity)) {
    packet.set_destX(dest->x);
    packet.set_destY(dest->y);
  }
  packet.set_command(computedValues.command);
  if (const auto* target = entitySystem.try_get_component<Component::Target>(entity); target && target->target != entt::null) {
      const auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(target->target);
      packet.set_targetId(basicInfo.id);
  } else {
      packet.set_targetId(0);
  }
  packet.set_moveMode(computedValues.moveMode);
  packet.set_hp(life.hp);
  packet.set_teamId(basicInfo.teamId);
  packet.set_statusFlag(computedValues.statusFlag);
  packet.set_npcId(mob.id);
  packet.set_questId(mob.quest);

  return packet;
}

RoseCommon::Packet::SrvDropItem CMapClient::create_srv_drop_item(const EntitySystem& entitySystem, Entity entity) {
    const auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(entity);
    const auto& pos = entitySystem.get_component<Component::Position>(entity);

    auto packet = Packet::SrvDropItem::create();
    packet.set_x(pos.x);
    packet.set_y(pos.y);
    packet.set_item(entitySystem.item_to_item<Packet::SrvDropItem>(entity));
    packet.set_id(basicInfo.id);
    if (const auto* owner = entitySystem.try_get_component<Component::Owner>(entity); owner && owner->owner != entt::null) {
        const auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(owner->owner);
        packet.set_ownerId(basicInfo.id);
    }
    return packet;
}

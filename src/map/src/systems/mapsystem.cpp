#include "cli_changemapreq.h"
#include "srv_changemapreply.h"
#include "srv_dropitem.h"
#include "srv_playerchar.h"
#include "srv_npcchar.h"
#include "cmapclient.h"
#include "cmapserver.h"
#include "systems/chatsystem.h"
#include "systems/mapsystem.h"

using namespace Systems;
using namespace RoseCommon;

MapSystem::MapSystem(SystemManager &m) : System(m) {
  m.registerDispatcher(ePacketType::PAKCS_CHANGE_MAP_REQ, &MapSystem::processChangeMapReq);
}

void MapSystem::update(EntityManager &, std::chrono::milliseconds) {}

void MapSystem::processChangeMapReq(CMapClient &client, Entity entity, const RoseCommon::CliChangeMapReq &) {
  logger_->trace("MapSystem::processChangeMapReq");

  auto advanced = entity.component<AdvancedInfo>();
  auto basic = entity.component<BasicInfo>();
  auto info = entity.component<CharacterInfo>();

  client.send(makePacket<ePacketType::PAKWC_CHANGE_MAP_REPLY>(getId(entity), advanced->hp_, advanced->mp_, basic->xp_,
                                                               info->penaltyXp_, std::time(nullptr), 0));
  manager_.get<Systems::ChatSystem>()->sendMsg(entity, "You are client " + std::to_string(getId(entity)));
  manager_.send(entity, CMapServer::eSendType::NEARBY_BUT_ME,
                                         makePacket<ePacketType::PAKWC_PLAYER_CHAR>(entity));
  const auto &movement = manager_.get<Systems::MovementSystem>();
  auto &manager = manager_.getEntityManager();
  for (Entity e : manager.entities_with_components<BasicInfo, AdvancedInfo>()) {
    if (!movement.is_nearby(entity, e)) continue; // do not send non nearby stuff
    basic = e.component<BasicInfo>();
    if (basic->ownerId_ == 0 && e.component<Npc>()) {
        client.send(makePacket<ePacketType::PAKWC_NPC_CHAR>(e));
    } else if (e.component<Npc>()) {
        client.send(makePacket<ePacketType::PAKWC_MOB_CHAR>(e));
    } else {
        if (e != entity && basic->isOnMap_.load())
            client.send(makePacket<ePacketType::PAKWC_PLAYER_CHAR>(e));
    }
  }
  for (Entity e : manager.entities_with_components<Item>()) {
    if (!movement.is_nearby(entity, e) continue; // do not send non nearby stuff
    client.send(makePacket<ePacketType::PAKWC_DROP_ITEM>(e));
  }
  entity.component<BasicInfo>()->isOnMap_.store(true);
}

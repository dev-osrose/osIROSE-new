#include <cmath>
#include <algorithm>

#include "config.h"
#include "cmapserver.h"
#include "cmapclient.h"
#include "packetfactory.h"
#include "cli_mousecmd.h"
#include "cli_stopmoving.h"
#include "srv_mousecmd.h"
#include "srv_stopmoving.h"
#include "srv_teleportreply.h"
#include "srv_switchserver.h"

#include "systems/combat_system.h"

using namespace Systems;
using namespace RoseCommon;

CombatSystem::CombatSystem(SystemManager &manager) : System(manager) {
  //manager.registerDispatcher(ePacketType::PAKCS_MOUSE_CMD, &CombatSystem::processMove);
  //manager.registerDispatcher(ePacketType::PAKCS_STOP_MOVING, &CombatSystem::stopMoving);
}

void CombatSystem::update(EntityManager &es, std::chrono::milliseconds dt) {
  for (Entity entity : es.entities_with_components<AdvancedInfo, Stats>()) {
    AdvancedInfo *advanced = entity.component<AdvancedInfo>();
    
    if(advanced->hp_ <= 0) {
      advanced->hp_ = 0;
      
      // send dead stoof
    }
    // float dx = destination->x_ - position->x_;
    // float dy = destination->y_ - position->y_;
    // float distance = std::sqrt(dx * dx + dy * dy);
    // destination->dist_ = distance;
    // float speed = advanced->runSpeed_;
    // std::chrono::milliseconds ntime{static_cast<int>(1000.f * distance / speed)};
    // float old_x = position->x_;
    // float old_y = position->y_;
    // if (ntime <= dt || distance == 0) {
    //   position->x_ = destination->x_;
    //   position->y_ = destination->y_;
    //   entity.remove<Destination>();
    // } else {
    //   auto delta = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(dt);
    //   position->x_ += dx * delta / ntime;
    //   position->y_ += dy * delta / ntime;
    // }
    updateHP(entity);
  }
}

void CombatSystem::apply_damage(Entity entity, uint32_t damage) {
  if (!entity) return;
  
  auto stats = entity.component<Stats>();
  auto advanced = entity.component<AdvancedInfo>();
  
  int32_t newHp = advanced->hp_;
  if(stats->maxHp_ < damage) {
    newHp = 0;
  }
  else {
    if(newHp - damage < 0) {
      newHp = 0;
    } else {
      newHp =- damage;
    }
  }
  advanced->hp_ = newHp;
  entity.component<AdvancedInfo>()->hp_ = newHp;

  // if (auto client = getClient(entity))
  //   manager_.send(entity, CMapServer::eSendType::NEARBY, makePacket<ePacketType::PAKWC_SET_HP_AND_MP>(entity, 0, advanced->mp_));
}

void CombatSystem::updateHP(Entity entity) {
    // logger_->trace("CombatSystem::updateHP");
    // logger_->debug("Teleporting {} to {} {} {}", getId(entity));
    // entity.component<BasicInfo>()->targetId_ = 0; // to avoid doing anything in the destination callback
    // entity.remove<Destination>();
    // auto pos = entity.component<Position>();
    // pos->x_ = x;
    // pos->y_ = y;
    // if (pos->map_ == map_id) {
    //     getClient(entity)->send(makePacket<ePacketType::PAKWC_TELEPORT_REPLY>(entity));
    // } else {
    //     if (auto client = getClient(entity)) {
    //         pos->map_ = map_id;
    //         auto &config = Core::Config::getInstance();
    //         client->switch_server();
    //         client->send(makePacket<ePacketType::PAKCC_SWITCH_SERVER>(
    //             config.mapServer().clientPort + map_id,
    //             client->get_session_id(),
    //             0,
    //             config.serverData().ip));
    //         client->switch_server(); // this allow us to not remove the online bit for this client as it's only moving around servers
    //         manager_.saveCharacter(entity); // force save
    //     }
    // }
}

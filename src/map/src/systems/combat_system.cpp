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
    updateHP(entity, dt);
    
    if(advanced->hp_ <= 0) {
      advanced->hp_ = 0;
      
      // send dead stoof
    }
  }
}

void CombatSystem::apply_damage(Entity defender, Entity attacker, int32_t damage) {
  if (!defender || !attacker) return;
  
  //auto stats = defender.component<Stats>();
  auto nDamage = defender.component<Damage>();
  //auto advanced = defender.component<AdvancedInfo>();
  
  if(!nDamage) {
   defender.assign<Damage>();
   nDamage = defender.component<Damage>();
  }
  
  //TODO: Replace 0 with the method of attack
  nDamage->addDamage(attacker, 0, damage);

  // if (auto client = getClient(defender))
  //   manager_.send(defender, CMapServer::eSendType::NEARBY, makePacket<ePacketType::PAKWC_SET_HP_AND_MP>(defender, 0, advanced->mp_));
}

void CombatSystem::apply_damage(Entity defender, int32_t damage) {
  if (!defender) return;
  
  //auto stats = defender.component<Stats>();
  auto nDamage = defender.component<Damage>();
  //auto advanced = defender.component<AdvancedInfo>();
  
  if(!nDamage) {
   defender.assign<Damage>();
   nDamage = defender.component<Damage>();
  }
  
  //TODO: Replace 0 with the method of attack
  nDamage->addDamage(defender, 0, damage);
}

void CombatSystem::updateHP(Entity entity, std::chrono::milliseconds dt) {
  if (!entity) return;
  
  //auto stats = entity.component<Stats>();
  auto damage = entity.component<Damage>();
  auto advanced = entity.component<AdvancedInfo>();
  
  //TODO: Calculate defensive buffs before doing damage
  //TODO: Calculate natural and magic health regen
  if(damage) {
    //TODO: Calculate each real damage done for each attack
    
    int32_t adjusted_hp = advanced->hp_;
    uint32_t total_applied_damage = 0;
    for(auto attack : damage->damage_)
    {
      if(adjusted_hp - attack.value_ <= 0) {
        total_applied_damage = attack.value_ + (adjusted_hp - attack.value_);
        adjusted_hp = 0;
        //TODO: Credit this attacker as the one who killed this entity.
        break;
      } else {
        total_applied_damage = attack.value_;
        adjusted_hp -= attack.value_;
      }
      
      //TODO: Store damage applied for this attacker for later (exp distributuion)
      //TODO: Send damage packet here
      // if (auto client = getClient(entity))
      //   manager_.send(entity, CMapServer::eSendType::NEARBY, makePacket<ePacketType::PAKWC_DAMAGE>(entity, attack.attacker_, attack.value_));
    }
    
    // Last sanity check to make sure our HP is not less then 0
    if(adjusted_hp < 0) {
      adjusted_hp = 0;
    }
    
    advanced->hp_ = adjusted_hp;
    entity.component<AdvancedInfo>()->hp_ = adjusted_hp;
    
    //TODO: Send HP update here
    // if (auto client = getClient(entity))
    //   manager_.send(entity, CMapServer::eSendType::NEARBY, makePacket<ePacketType::PAKWC_SET_HP_AND_MP>(entity, 0, advanced->mp_));
  }
}

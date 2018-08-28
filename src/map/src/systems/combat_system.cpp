#include <cmath>
#include <algorithm>

#include "config.h"
#include "cmapserver.h"
#include "cmapclient.h"
#include "packetfactory.h"
#include "cli_attack.h"
#include "cli_hpreq.h"
#include "srv_attack.h"
#include "srv_damage.h"
#include "srv_hpreply.h"
#include "srv_sethpandmp.h"

#include "systems/combat_system.h"

using namespace Systems;
using namespace RoseCommon;

CombatSystem::CombatSystem(SystemManager &manager) : System(manager) {
  manager.registerDispatcher(ePacketType::PAKCS_ATTACK, &CombatSystem::processAttack);
  manager.registerDispatcher(ePacketType::PAKCS_HP_REQ, &CombatSystem::processHpRequest);
}

void CombatSystem::update(EntityManager &es, std::chrono::milliseconds dt) {
  for (Entity entity : es.entities_with_components<AdvancedInfo, Damage>()) {
    updateHP(entity, dt);
  }
}

void CombatSystem::apply_damage(Entity defender, Entity attacker, int32_t damage) {
  if (!defender || !attacker) return;
  
  //auto stats = defender.component<Stats>();
  auto nDamage = defender.component<Damage>();
  //auto advanced = defender.component<AdvancedInfo>();
  
  if(!nDamage) {
    logger_->debug("Adding damage component to entity");
    defender.assign<Damage>();
    nDamage = defender.component<Damage>();
  }
  
  // We aren't applying static damage, calculate it now
  if(damage <= 0) {
    //TODO: Calculate each real damage done by setting value_
  }
  
  //TODO: Replace 0 with the method of attack
  nDamage->addDamage(attacker, 0, damage);
}

void CombatSystem::apply_damage(Entity defender, int32_t damage) {
  if (!defender) return;
  
  //auto stats = defender.component<Stats>();
  auto nDamage = defender.component<Damage>();
  //auto advanced = defender.component<AdvancedInfo>();
  
  if(!nDamage) {
    logger_->debug("Adding damage component to entity");
    defender.assign<Damage>();
    nDamage = defender.component<Damage>();
  }
  
  // We aren't applying static damage, calculate it now
  if(damage <= 0) {
    //TODO: Calculate each real damage done by setting value_
  }
  
  //TODO: Replace 0 with the method of attack
  nDamage->addDamage(defender, 0, damage);
}

void CombatSystem::updateHP(Entity entity, std::chrono::milliseconds dt) {
  if (!entity) return;
  
  //auto stats = entity.component<Stats>(); // max hp and mp
  auto damage = entity.component<Damage>();
  auto advanced = entity.component<AdvancedInfo>();
  
  //TODO: Calculate defensive buffs before doing damage
  //TODO: Calculate natural and magic health regen
  if(damage) {
    int32_t adjusted_hp = advanced->hp_;
    uint32_t total_applied_damage = 0;
    for(auto attack : damage->damage_)
    {
      Entity other;
      if (!(other = manager_.getEntity(attack.attacker_))) {
        continue;
      }
      
      logger_->debug("Applying damage to entity");
      
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
      //TODO: If the entity is dead send the packet to everyone (with drop info as needed)
      // else send to the attacker, the defender and the defender's party
      if(adjusted_hp <= 0) {
        if (auto client = getClient(entity))
          manager_.send(entity, CMapServer::eSendType::NEARBY, makePacket<ePacketType::PAKWC_DAMAGE>(other, entity, attack.value_)); //TODO: Send item info here...
      } else {
        if (auto client = getClient(entity))
          client->send(makePacket<ePacketType::PAKWC_DAMAGE>(other, entity, attack.value_));
      }
    }
    
    // Last sanity check to make sure our HP is not less then 0
    if(adjusted_hp < 0) {
      adjusted_hp = 0;
    }
    
    //advanced->hp_ = adjusted_hp;
    entity.component<AdvancedInfo>()->hp_ = adjusted_hp;
    
    //TODO: Send HP update here
    if (auto client = getClient(entity))
      manager_.send(entity, CMapServer::eSendType::NEARBY, makePacket<ePacketType::PAKWC_SET_HP_AND_MP>(entity));
  }
}

void CombatSystem::processAttack(CMapClient &client, Entity entity, const RoseCommon::CliAttack &packet)
{
  logger_->trace("CombatSystem::processAttack start");
  if (!entity) return;
  
  Entity other;
  if (!(other = manager_.getEntity(packet.targetId()))) {
    logger_->debug("Client {} requested to engage combat with an non-existing entity {}", getId(entity), packet.targetId());
    return;
  }
  
  //TODO: Set the other entity as the destination
  manager_.send(entity, CMapServer::eSendType::NEARBY, makePacket<ePacketType::PAKWC_ATTACK>(entity, other));
}

void CombatSystem::processHpRequest(CMapClient &client, Entity entity, const RoseCommon::CliHpReq &packet)
{
  logger_->trace("CombatSystem::processHpRequest start");
  if (!entity) return;
  
  Entity other;
  if (!(other = manager_.getEntity(packet.targetId()))) {
    logger_->debug("Client {} requested the hp of a non existing entity {}", getId(entity), packet.targetId());
    return;
  }
  
  client.send(makePacket<ePacketType::PAKWC_HP_REPLY>(other));
}

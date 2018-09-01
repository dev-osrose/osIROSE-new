#include <cmath>
#include <algorithm>

#include "config.h"
#include "cmapserver.h"
#include "cmapclient.h"
#include "packetfactory.h"
#include "dataconsts.h"

#include "cli_attack.h"
#include "cli_hpreq.h"
#include "cli_revivereq.h"
#include "srv_attack.h"
#include "srv_damage.h"
#include "srv_hpreply.h"
#include "srv_sethpandmp.h"

#include "systems/movementsystem.h"
#include "systems/combat_system.h"

using namespace Systems;
using namespace RoseCommon;
using namespace ReviveReq;

CombatSystem::CombatSystem(SystemManager &manager) : System(manager) {
  manager.registerDispatcher(ePacketType::PAKCS_ATTACK, &CombatSystem::processAttack);
  manager.registerDispatcher(ePacketType::PAKCS_HP_REQ, &CombatSystem::processHpRequest);
  manager.registerDispatcher(ePacketType::PAKCS_REVIVE_REQ, &CombatSystem::processReviveRequest);
}

void CombatSystem::update(EntityManager &es, std::chrono::milliseconds dt) {
  for (Entity entity : es.entities_with_components<AdvancedInfo, BasicInfo, Stats, AdvancedInfo>()) {
    updateHP(entity, dt);
  }
}

void CombatSystem::apply_damage(Entity defender, Entity attacker, int32_t damage) {
  if (!defender || !attacker) return;
  
  //auto stats = defender.component<Stats>();
  auto attackerBasic = attacker.component<BasicInfo>();
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
  nDamage->addDamage(attackerBasic->id_, DAMAGE_ACTION_ATTACK, damage);
}

void CombatSystem::apply_damage(Entity defender, int32_t damage) {
  if (!defender) return;
  
  auto basic = defender.component<BasicInfo>();
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
  nDamage->addDamage(basic->id_, DAMAGE_ACTION_ATTACK, damage);
}

void CombatSystem::updateHP(Entity entity, std::chrono::milliseconds dt) {
  if (!entity) return;
  
  auto basic = entity.component<BasicInfo>();
  auto stats = entity.component<Stats>();
  auto damage = entity.component<Damage>();
  auto advanced = entity.component<AdvancedInfo>();
  
  //logger_->debug("Updating HP for {}", basic->name_);
  
  //TODO: Calculate defensive buffs before doing damage
  //TODO: Calculate natural and magic health regen
  // if(false)
  // {
  //   int stanceModifier = (basic->command_ == BasicInfo::SIT ? 4 : 1); // This should be if sitting
  //   {
  //     // Regen HP
  //     int32_t amount = (int32_t)std::ceil(stats->maxHp_ * 0.02);
  //     amount = amount * stanceModifier;
  //     //TODO: update amount based on equipment values
  //     //TODO: Take into account HP regen buffs
    
  //     advanced->hp_ += amount;
  //   }
  
  //   {
  //     // Regen MP
  //     int32_t amount = (int32_t)std::ceil(stats->maxMp_ * 0.02);
  //     amount = amount * stanceModifier;
  //     //TODO: update amount based on equipment values
  //     //TODO: Take into account HP regen buffs

  //     advanced->mp_ += amount;
  //   }
    
  //   if(advanced->hp_ > stats->maxHp_) {
  //     advanced->hp_ = stats->maxHp_;
  //   }

  //   if(advanced->mp_ > stats->maxMp_) {
  //     advanced->mp_ = stats->maxMp_;
  //   }
  //   manager_.send(entity, CMapServer::eSendType::NEARBY, makePacket<ePacketType::PAKWC_SET_HP_AND_MP>(entity));
  // }

  if(damage) {
    int32_t adjusted_hp = advanced->hp_;
    uint32_t total_applied_damage = 0;
    for(auto attack : damage->damage_)
    {
      Entity other;
      if (!(other = manager_.getEntity(attack.attacker_))) {
        logger_->debug("Attacking entity doesn't exist. Skipping...");
        continue;
      }
      
      logger_->debug("Applying damage to entity {} {}", basic->name_, basic->id_);
      
      if((adjusted_hp - attack.value_) <= 0) {
        logger_->debug("Entity {} {} will die from {} damage", basic->name_, basic->id_, attack.value_);
        total_applied_damage = attack.value_ + adjusted_hp;
        adjusted_hp = 0;
        //TODO: Credit this attacker as the one who killed this entity.
      } else {
        total_applied_damage += attack.value_;
        adjusted_hp -= attack.value_;
      }
      
      //TODO: Store damage applied for this attacker for later (exp distributuion)
      
      //TODO: Send damage packet here
      //TODO: If the entity is dead send the packet to everyone (with drop info as needed)
      // else send to the attacker, the defender and the defender's party
      if(adjusted_hp <= 0) {
        //TODO: Get dropped item data here and send it with the DAMAGE packet
        attack.action_ &= ~DAMAGE_ACTION_HIT;
        attack.action_ |= DAMAGE_ACTION_DEAD;
        manager_.send(entity, CMapServer::eSendType::NEARBY, makePacket<ePacketType::PAKWC_DAMAGE>(other, entity, attack.value_ + 30000, attack.action_));
        entity.remove<Damage>();
      } else {
        logger_->debug("applied {} damage to entity {} {}.", attack.value_, basic->name_, basic->id_);
        if (auto client = getClient(entity))
          client->send(makePacket<ePacketType::PAKWC_DAMAGE>(other, entity, attack.value_, attack.action_));
        else if (auto client = getClient(other))
          client->send(makePacket<ePacketType::PAKWC_DAMAGE>(other, entity, attack.value_, attack.action_));
      }
    }
    damage->damage_.clear();
    entity.component<AdvancedInfo>()->hp_ = adjusted_hp;
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

Entity CombatSystem::get_closest_spawn(Entity player) {
  auto position = player.component<Position>();
  
  Entity closest = {};
  double closestDist = 999999999999;
  
  for (Entity entity : manager_.getEntityManager().entities_with_components<BasicInfo, Position, PlayerSpawn>()) {
    auto spawnPosition = entity.component<Position>();
    
    if(spawnPosition->map_ != position->map_) continue;
    
    float dx = spawnPosition->x_ - position->x_;
    float dy = spawnPosition->y_ - position->y_;
    float distance = std::sqrt(dx * dx + dy * dy);
    
    if(closestDist > distance) {
      closest = entity;
      closestDist = distance;
    }
  }
  
  return closest;
}

Entity CombatSystem::get_saved_spawn(Entity player) {
  auto position = player.component<Position>();

  for (Entity entity : manager_.getEntityManager().entities_with_components<BasicInfo, Position, PlayerSpawn>()) {
    auto spawnInfo = entity.component<PlayerSpawn>();
    if(spawnInfo->type_ == PlayerSpawn::LOGIN_POINT) {
      auto spawnPosition = entity.component<Position>();

      if(spawnPosition->map_ == position->spawn_)
        return entity;
    }
  }
  
  logger_->trace("CombatSystem::get_saved_spawn unable to find the saved spawn point");
  return {};
}

Entity CombatSystem::get_start_spawn() {
  for (Entity entity : manager_.getEntityManager().entities_with_components<BasicInfo, Position, PlayerSpawn>()) {
    auto spawnPosition = entity.component<Position>();
    auto spawninfo = entity.component<PlayerSpawn>();
    
    if(spawninfo->type_ == PlayerSpawn::LOGIN_POINT)
      return entity;
  }
  
  return {};
}

#define	RAND(value)				( rand() % value ) // This is only temp
void CombatSystem::processReviveRequest(CMapClient &client, Entity entity, const RoseCommon::CliReviveReq &packet)
{
  logger_->trace("CombatSystem::processReviveRequest start");
  if (!entity) return;
  
  auto basic = entity.component<BasicInfo>();
  auto stats = entity.component<Stats>();
  auto position = entity.component<Position>();
  auto charinfo = entity.component<CharacterInfo>();
  
  if(!charinfo) {
    logger_->warn("CombatSystem::processReviveRequest recved revive request for an entity that doesn't have 'CharacterInfo'");
    return;
  }
  
  uint16_t map_id = position->map_;
  float x = 0.f, y = 0.f;
  
  switch(packet.reviveType())
  {
    case REVIVE_POS:
    {
      if (Entity e = get_closest_spawn(entity); e) {
        auto dest = e.component<Position>();
        x = dest->x_ + (RAND(1001) - 500);
        y = dest->y_ + (RAND(1001) - 500);
      }
      break;
    }
    case SAVE_POS:
    {
      //TODO: get the save location of the player
      //TOOD: Make sure our save location isn't on birth island if we already left it
      if (Entity e = get_saved_spawn(entity); e) {
        auto dest = e.component<Position>();
        
        if(dest->map_ == 20 && charinfo->job_) {
          if (Entity e = get_closest_spawn(entity); e) {
            auto dest = e.component<Position>();
            x = dest->x_ + (RAND(1001) - 500);
            y = dest->y_ + (RAND(1001) - 500);
          }
          break;
        }
        
        if(position->map_ != dest->map_) {
          map_id = dest->map_;
        }
        x = dest->x_ + (RAND(1001) - 500);
        y = dest->y_ + (RAND(1001) - 500);
      }
      break;
    }
    case START_POST:
    {
      //TODO: grab the start position of this map
      if (Entity e = get_start_spawn(); e) {
        auto dest = e.component<Position>();
        x = dest->x_ + (RAND(1001) - 500);
        y = dest->y_ + (RAND(1001) - 500);
      }
      break;
    }
    case CURRENT_POS:
      x = position->x_;
      y = position->y_;
      break;
    default:
      logger_->warn("CombatSystem::processReviveRequest player {} sent a revive type that doesn't exist...", basic->name_);
      return;
  }
  
  entity.component<AdvancedInfo>()->hp_ = (stats->maxHp_ * 0.3f);
  entity.component<AdvancedInfo>()->mp_ = 0;
  manager_.get<MovementSystem>()->teleport(entity, map_id, x, y);
}
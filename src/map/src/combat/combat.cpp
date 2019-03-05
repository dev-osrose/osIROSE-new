#include "combat/combat.h"

#include "random.h"
#include "logconsole.h"
#include "dataconsts.h"
#include "entity_system.h"

#include "srv_attack.h"
#include "srv_damage.h"
#include "srv_hp_reply.h"

#include "components/basic_info.h"
#include "components/destination.h"
#include "components/life.h"
#include "components/magic.h"
#include "components/player_spawn.h"
#include "components/position.h"
#include "components/target.h"
#include "components/damage.h"
#include "components/skills.h"
#include "components/stamina.h"
#include "components/stats.h"

#include <cmath>

using namespace RoseCommon;
using namespace RoseCommon::Packet;


void Combat::hp_request(EntitySystem& entitySystem, Entity entity, const CliHpReq& packet) {
  auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
  if (packet.get_targetId()) {
    Entity t = entitySystem.get_entity_from_id(packet.get_targetId());
    if (t != entt::null) {
      const auto& life = entitySystem.get_component<Component::Life>(t);
      
      auto p = SrvHpReply::create(packet.get_targetId(), life.hp);
      entitySystem.send_to(entity, p);
    }
  }
}


std::pair<float, float> Combat::get_range_position(const EntitySystem& entitySystem, Entity character, Entity target, float range) {
  const auto& char_pos = entitySystem.get_component<Component::Position>(character);
  const auto& target_pos = entitySystem.get_component<Component::Position>(target);
  std::pair<float, float> vector{char_pos.x - target_pos.x, char_pos.y - target_pos.y};
  const float length = sqrt(vector.first * vector.first + vector.second * vector.second);
  vector.first /= length;
  vector.second /= length;
  return {vector.first * range, vector.second * range};
}

float Combat::get_range_to(const EntitySystem& entitySystem, Entity character, Entity target) {
  const auto& char_pos = entitySystem.get_component<Component::Position>(character);
  const auto& target_pos = entitySystem.get_component<Component::Position>(target);
  
  const float dx = char_pos.x - target_pos.x;
  const float dy = char_pos.y - target_pos.y;
  
  return std::sqrt(dx * dx + dy * dy);
}

void Combat::attack(EntitySystem& entitySystem, Entity entity, const CliAttack& packet) {
  auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
  logger->trace("Combat::attack");
  logger->trace("entity {}, target {}", entity, packet.get_targetId());

  const auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(entity);
  const auto& pos = entitySystem.get_component<Component::Position>(entity);
  const auto& stats = entitySystem.get_component<Component::Stats>(entity);
  
  if (packet.get_targetId()) {
    Entity t = entitySystem.get_entity_from_id(packet.get_targetId());
    if (t != entt::null) {
      auto& target = entitySystem.add_or_replace_component<Component::Target>(entity);
      target.target = t;
      
      //TODO: Check distance to target, if not in attack range, move into max attack range
      if(get_range_to(entitySystem, entity, t) > 1)
      {
        auto& dest = entitySystem.add_or_replace_component<Component::Destination>(entity);
        auto npos = get_range_position(entitySystem, entity, t);
        dest.x = npos.first;
        dest.y = npos.second;
        dest.z = 0;
        
        const float dx = pos.x - dest.x;
        const float dy = pos.y - dest.y;
        dest.dist = std::sqrt(dx * dx + dy * dy);
      }
      
      // This packet acts as an attack and mouse_cmd all in one, we don't want the mouse_cmd portion 
      // of it as it can cause some issues with attack animations going off before it should
      // auto p = SrvAttack::create(basicInfo.id, packet.get_targetId());
      // p.set_x(dest.x);
      // p.set_y(dest.y);
      // p.set_distance(dest.dist);
      // entitySystem.send_nearby(entity, p);
    }
  }
}

void Combat::update(EntitySystem& entitySystem, Entity entity) {
  auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
  const auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(entity);
  const auto& pos = entitySystem.get_component<Component::Position>(entity);
  auto& life = entitySystem.get_component<Component::Life>(entity);
  
  //TODO:: Update buffs
  //TODO:: Update HP
  
  if(entitySystem.has_component<Component::Damage>(entity) == true)
  {
    auto& queuedDamage = entitySystem.get_component<Component::Damage>(entity);
    int32_t adjusted_hp = life.hp;
    uint32_t total_applied_damage = 0;
    
    for(auto attack : queuedDamage.damage_)
    {
      if(attack.apply_) {
        // We waited at least one update before applying damage, apply it now
        //TODO:: Apply damage to this entity
        Entity attacker = entitySystem.get_entity_from_id(attack.attacker_);
        logger->debug("Applying damage to entity {} {}", basicInfo.name, basicInfo.id);
        
        if((adjusted_hp - attack.value_) <= 0) {
          logger->debug("Entity {} {} will die from {} damage", basicInfo.name, basicInfo.id, attack.value_);
          total_applied_damage = attack.value_ + adjusted_hp;
          adjusted_hp = 0;
          //TODO: Credit this attacker as the one who killed this entity.
        } else {
          total_applied_damage += attack.value_;
          adjusted_hp -= attack.value_;
        }
        
        if(adjusted_hp <= 0) {
          //TODO: Get dropped item data here and send it with the DAMAGE packet
          attack.action_ &= ~DAMAGE_ACTION_HIT;
          attack.action_ |= DAMAGE_ACTION_DEAD;
          auto p = SrvDamage::create(attack.attacker_, basicInfo.id, attack.value_ + 30000, attack.action_);
          entitySystem.send_nearby(entity, p);
          
          // remove components that we can't have if we are dead!
          entitySystem.remove_component<Component::Damage>(entity);
          entitySystem.remove_component<Component::Destination>(entity);
          entitySystem.remove_component<Component::Target>(entity);
        } else {
          logger->debug("applied {} damage to entity {} {}.", attack.value_, basicInfo.name, basicInfo.id);
          
          auto p = SrvDamage::create(attack.attacker_, basicInfo.id, attack.value_, attack.action_);
          entitySystem.send_to(entity, p);
          entitySystem.send_to(attacker, p);
        }
        
        // Reset the damage value to 0 for next pass
        attack.value_ = 0;
      } else {
        // Apply any new damage on next update
        attack.apply_ = true;
      }
    }
  }
  
  // Check to see if we have a target component
  if(entitySystem.has_component<Component::Target>(entity) == true)
  {
    auto& target = entitySystem.get_component<Component::Target>(entity);
    // Are we in attack range?
    //TODO:: Update apply damage to target
  }
}

RoseCommon::Entity Combat::get_closest_spawn(EntitySystem& entitySystem, RoseCommon::Entity player) {
  auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
  logger->trace("Combat::get_closest_spawn start");
  const auto& position = entitySystem.get_component<Component::Position>(player);
  
  RoseCommon::Entity closest = {};
  float closestDist = 999999999999;
  
  for (RoseCommon::Entity entity : entitySystem.get_entities_with_components<Component::BasicInfo, Component::Position, Component::PlayerSpawn>()) {
    const auto& spawnPosition = entitySystem.get_component<Component::Position>(entity);
    
    if(spawnPosition.map != position.map) continue;
    
    float dx = spawnPosition.x - position.x;
    float dy = spawnPosition.y - position.y;
    float distance = std::sqrt(dx * dx + dy * dy);
    
    if(distance < closestDist) {
      closest = entity;
      closestDist = distance;
    }
  }
  logger->trace("Combat::get_closest_spawn end");
  return closest;
}

RoseCommon::Entity Combat::get_saved_spawn(EntitySystem& entitySystem, RoseCommon::Entity player) {
  auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
  logger->trace("Combat::get_saved_spawn start");
  const auto& position = entitySystem.get_component<Component::Position>(player);

  for (RoseCommon::Entity entity : entitySystem.get_entities_with_components<Component::BasicInfo, Component::Position, Component::PlayerSpawn>()) {
    const auto& spawninfo = entitySystem.get_component<Component::PlayerSpawn>(entity);
    if(spawninfo.type == Component::PlayerSpawn::RESPAWN_POINT) {
      const auto& spawnPosition = entitySystem.get_component<Component::Position>(entity);

      if(spawnPosition.map == position.spawn)
        return entity;
    }
  }
  
  logger->trace("Combat::get_saved_spawn unable to find the saved spawn point");
  return {};
}

RoseCommon::Entity Combat::get_start_spawn(EntitySystem& entitySystem) {
  auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
  logger->trace("Combat::get_start_spawn start");
  for (RoseCommon::Entity entity : entitySystem.get_entities_with_components<Component::BasicInfo, Component::Position, Component::PlayerSpawn>()) {
    const auto& spawninfo = entitySystem.get_component<Component::PlayerSpawn>(entity);
    
    if(spawninfo.type == Component::PlayerSpawn::START_POINT)
      return entity;
  }
  
  return {};
}

void Combat::revive(EntitySystem& entitySystem, Entity entity, const RoseCommon::Packet::CliReviveReq& packet) {
  auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
  logger->trace("Combat::revive");
  
  const auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(entity);
  const auto& pos = entitySystem.get_component<Component::Position>(entity);
  auto& life = entitySystem.get_component<Component::Life>(entity);
  auto& magic = entitySystem.get_component<Component::Magic>(entity);
  
  uint16_t map_id = pos.map;
  float x = 0.f, y = 0.f;
  
  switch(packet.get_reviveType())
  {
    case CliReviveReq::ReviveRequest::REVIVE_POSITION: 
    {
      if (Entity e = get_closest_spawn(entitySystem, entity); e) {
        auto& dest = entitySystem.get_component<Component::Position>(e);
        x = dest.x + (Core::Random::getInstance().get_uniform(0, 1001) - 500);
        y = dest.y + (Core::Random::getInstance().get_uniform(0, 1001) - 500);
      }
      break;
    }
    case CliReviveReq::ReviveRequest::SAVE_POSITION: 
    {
      if (Entity e = get_saved_spawn(entitySystem, entity); e) {
        auto& dest = entitySystem.get_component<Component::Position>(e);
        
        if(dest.map == 20 && basicInfo.job) {
          if (Entity e = get_closest_spawn(entitySystem, entity); e) {
            auto& dest = entitySystem.get_component<Component::Position>(e);
            x = dest.x + (Core::Random::getInstance().get_uniform(0, 1001) - 500);
            y = dest.y + (Core::Random::getInstance().get_uniform(0, 1001) - 500);
          }
          break;
        }
        
        if(pos.map != dest.map) {
          map_id = dest.map;
        }
        x = dest.x + (Core::Random::getInstance().get_uniform(0, 1001) - 500);
        y = dest.y + (Core::Random::getInstance().get_uniform(0, 1001) - 500);
      }
      break;
    }
    case CliReviveReq::ReviveRequest::START_POSITION: 
    {
      if (Entity e = get_start_spawn(entitySystem); e) {
        auto& dest = entitySystem.get_component<Component::Position>(e);
        x = dest.x + (Core::Random::getInstance().get_uniform(0, 1001) - 500);
        y = dest.y + (Core::Random::getInstance().get_uniform(0, 1001) - 500);
      }
      break;
    }
    case CliReviveReq::ReviveRequest::CURRENT_POSITION: 
    {
      x = pos.x;
      y = pos.y;
      break;
    }
    default:
    {
        logger->warn("Combat::revive player {} sent a revive type that doesn't exist...", basicInfo.name);
      break;
    }
  }
  
  life.hp = (life.maxHp * 0.3f);
  magic.mp = 0;

  entitySystem.teleport_entity(entity, x, y, map_id);
}
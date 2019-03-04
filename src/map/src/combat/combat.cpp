#include <utility>
#include "combat/combat.h"

#include "logconsole.h"
#include "entity_system.h"

#include "srv_attack.h"

#include "components/basic_info.h"
#include "components/destination.h"
#include "components/position.h"
#include "components/target.h"
#include "components/damage.h"

#include <cmath>

using namespace RoseCommon;
using namespace RoseCommon::Packet;

std::pair<float, float> get_range_position(const EntitySystem& entitySystem, Entity character, Entity target, float range) {
    const auto& char_pos = entitySystem.get_component<Component::Position>(character);
    const auto& target_pos = entitySystem.get_component<Component::Position>(target);
    std::pair<float, float> vector{char_pos.x - target_pos.x, char_pos.y - target_pos.y};
    const float length = sqrt(vector.first * vector.first + vector.second * vector.second);
    vector.first /= length;
    vector.second /= length;
    return {vector.first * range, vector.second * range};
}

void Combat::attack(EntitySystem& entitySystem, Entity entity, const CliAttack& packet) {
  auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
  logger->trace("Combat::attack");
  logger->trace("entity {}, target {}", entity, packet.get_targetId());

  const auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(entity);
  const auto& pos = entitySystem.get_component<Component::Position>(entity);
  
  if (packet.get_targetId()) {
    Entity t = entitySystem.get_entity_from_id(packet.get_targetId());
    if (t != entt::null) {
      auto& target = entitySystem.add_or_replace_component<Component::Target>(entity);
      target.target = t;
      
      //TODO: Check distance to target, if not in attack range, move into max attack range
      // auto& dest = entitySystem.add_or_replace_component<Component::Destination>(entity);
      // dest.x = packet.get_x();
      // dest.y = packet.get_y();
      // dest.z = 0;
      
      // const float dx = pos.x - dest.x;
      // const float dy = pos.y - dest.y;
      // dest.dist = std::sqrt(dx * dx + dy * dy);
      
      // This packet acts as an attack and mouse_cmd all in one
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
  //logger->trace("Combat::update");
  const auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(entity);
  const auto& pos = entitySystem.get_component<Component::Position>(entity);
  const auto& damage = entitySystem.get_component<Component::Damage>(entity);
  
  //TODO:: Update buffs
  //TODO:: Update HP
  
  // Check to see if we have a target component
  if(entitySystem.has_component<Component::Target>(entity) == true)
  {
    const auto& target = entitySystem.get_component<Component::Target>(entity);
    // Are we in attack range?
    //TODO:: Update apply damage to target
  }
}

#define	RAND(value) ( rand() % value ) // This is only temp
void Combat::revive(EntitySystem& entitySystem, Entity entity, const RoseCommon::Packet::CliReviveReq& packet) {
  auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
  logger->trace("Combat::revive");
  
  const auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(entity);
  const auto& pos = entitySystem.get_component<Component::Position>(entity);
  
  uint16_t map_id = pos.map;
  float x = 0.f, y = 0.f;
  
  switch(packet.get_reviveType())
  {
    case CliReviveReq::ReviveRequest::REVIVE_POSITION: 
    {
      // auto& dest = entitySystem.get_component<Component::Position>(e);
      // x = dest.x + (RAND(1001) - 500);
      // y = dest.y + (RAND(1001) - 500);
      break;
    }
    case CliReviveReq::ReviveRequest::SAVE_POSITION: 
    {
      break;
    }
    case CliReviveReq::ReviveRequest::START_POSITION: 
    {
      break;
    }
    case CliReviveReq::ReviveRequest::CURRENT_POSITION: 
    {
      break;
    }
    default:
    {
        logger->warn("Combat::revive player {} sent a revive type that doesn't exist...", basicInfo.name);
      break;
    }
  }
  
  //TODO:: Do teleport here
}
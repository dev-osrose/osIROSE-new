#include <algorithm>
#include "combat/combat.h"

#include "random.h"
#include "logconsole.h"
#include "dataconsts.h"
#include "entity_system.h"
#include "items/inventory.h"

#include "srv_attack.h"
#include "srv_damage.h"
#include "srv_levelup.h"
#include "srv_hp_reply.h"
#include "srv_mouse_cmd.h"
#include "srv_set_hp_and_mp.h"
#include "srv_set_exp.h"

#include "components/basic_info.h"
#include "components/computed_values.h"
#include "components/destination.h"
#include "components/level.h"
#include "components/life.h"
#include "components/lua.h"
#include "components/magic.h"
#include "components/mob.h"
#include "components/npc.h"
#include "components/player_spawn.h"
#include "components/position.h"
#include "components/target.h"
#include "components/combat.h"
#include "components/skills.h"
#include "components/stamina.h"
#include "components/stats.h"

#include "utils/calculation.h"

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

void Combat::check_for_level_up(EntitySystem& entitySystem, Entity entity, uint16_t sourceId) {
  auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
  auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(entity);
  auto& attackerLevel = entitySystem.get_component<Component::Level>(entity);

  uint16_t addedStatPoints = 0;
  uint16_t addedSkillPoints = 0;

  bool leveled = false;
  auto exp_required = get_exp_to_level(attackerLevel.level+1);
  while ((int64_t)attackerLevel.xp >= exp_required) {
    int64_t new_exp = attackerLevel.xp - exp_required;
    attackerLevel.xp = new_exp;
    ++attackerLevel.level;
    leveled = true;

    //TODO:: adjust our hp and mp based on our level here
    //TODO:: give the player attr and skill points
    addedStatPoints += 0;
    addedSkillPoints += 0;

    exp_required = get_exp_to_level(attackerLevel.level+1);
  }

  if(leveled == true) {
    auto pLevel = SrvLevelup::create(basicInfo.id, attackerLevel.level, attackerLevel.xp, addedStatPoints, addedSkillPoints);
    entitySystem.send_to(entity, pLevel);
  }

  int16_t current_stamina = 0;
  if(entitySystem.has_component<Component::Stamina>(entity) == true) {
    auto& attackerStamina = entitySystem.get_component<Component::Stamina>(entity);
    current_stamina = attackerStamina.stamina;
  }

  auto pExp = SrvSetExp::create(attackerLevel.xp, current_stamina);
  pExp.set_source_id(sourceId);
  entitySystem.send_to(entity, pExp);
}


std::pair<float, float> Combat::get_range_position(const EntitySystem& entitySystem, Entity character, Entity target, float range) {
  auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
  const auto& char_pos = entitySystem.get_component<Component::Position>(character);
  const auto& target_pos = entitySystem.get_component<Component::Position>(target);
  std::pair<float, float> vector{char_pos.x - target_pos.x, char_pos.y - target_pos.y};
  const float length = sqrt(vector.first * vector.first + vector.second * vector.second);

  if(length <= range) {
    logger->debug("within range, returning x,y {},{}", char_pos.x, char_pos.y);
    return {char_pos.x, char_pos.y};
  }

  vector.first /= length;
  vector.second /= length;
  logger->debug("out of range, returning x,y {},{}", (vector.first * range) + target_pos.x, (vector.second * range) + target_pos.y);
  return {(vector.first * range) + target_pos.x, (vector.second * range) + target_pos.y};
}

float Combat::get_range_to(const EntitySystem& entitySystem, Entity character, Entity target) {
  const auto& char_pos = entitySystem.get_component<Component::Position>(character);
  const auto& target_pos = entitySystem.get_component<Component::Position>(target);

  const float dx = char_pos.x - target_pos.x;
  const float dy = char_pos.y - target_pos.y;

  return std::sqrt(dx * dx + dy * dy) * 0.01f;
}

void Combat::attack(EntitySystem& entitySystem, Entity entity, const CliAttack& packet) {
  auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
  logger->trace("Combat::attack");
  logger->trace("entity {}, target {}", entity, packet.get_targetId());

  const auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(entity);
  const auto& pos = entitySystem.get_component<Component::Position>(entity);
  const auto& values = entitySystem.get_component<Component::ComputedValues>(entity);
  //const auto& stats = entitySystem.get_component<Component::Stats>(entity);

  if (packet.get_targetId()) {
    Entity t = entitySystem.get_entity_from_id(packet.get_targetId());
    if (t != entt::null) {
      auto& target = entitySystem.add_or_replace_component<Component::Target>(entity);
      target.target = t;

      logger->debug("distance to target is {}", get_range_to(entitySystem, entity, t));

      //TODO: Check distance to target, if not in attack range, move into max attack range
      if(get_range_to(entitySystem, entity, t) > values.attackRange)
      {
        auto& dest = entitySystem.add_or_replace_component<Component::Destination>(entity);
        auto npos = get_range_position(entitySystem, entity, t, ((values.attackRange * 100) - 0.1f));
        dest.x = npos.first;
        dest.y = npos.second;
        dest.z = 0;

        const float dx = pos.x - dest.x;
        const float dy = pos.y - dest.y;
        dest.dist = std::sqrt(dx * dx + dy * dy);

        //auto p = SrvMouseCmd::create(basicInfo.id);
        auto p = SrvAttack::create(basicInfo.id, packet.get_targetId());
        //p.set_targetId(0);
        p.set_x(dest.x);
        p.set_y(dest.y);
        p.set_z(0);
        entitySystem.send_nearby(entity, p);
      }
      else
      {
        // This packet acts as an attack and mouse_cmd all in one, we don't want the mouse_cmd portion
        // of it as it can cause some issues with attack animations going off before it should
        auto p = SrvAttack::create(basicInfo.id, packet.get_targetId());
        p.set_x(pos.x);
        p.set_y(pos.y);
        p.set_z(0);
        entitySystem.send_nearby(entity, p);
      }
    }
  }
}

void Combat::update(EntitySystem& entitySystem, Entity entity, uint32_t dt) {
  auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
  const auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(entity);
  //const auto& pos = entitySystem.get_component<Component::Position>(entity);
  const auto& level = entitySystem.get_component<Component::Level>(entity);
  auto& life = entitySystem.get_component<Component::Life>(entity);
  auto& values = entitySystem.get_component<Component::ComputedValues>(entity);

  //TODO:: Update buffs
  //TODO:: Update HP
  values.regenDt += dt;
  if(values.regenDt >= 4000) // Regen happens every 4 seconds
  {
    values.regenDt = 0;
    uint32_t hp = life.hp, mp = 0;
    int stanceModifier = (values.command == RoseCommon::Command::SIT ? 4 : 1); // This should be if sitting
    if(life.hp > 0 && life.hp != life.maxHp)
    {
      int32_t amount = (int32_t)std::ceil(life.maxHp * 0.02);
      amount = amount * stanceModifier;
      //TODO: update amount based on equipment values
      //TODO: Take into account HP regen buffs
      life.hp += amount;

      if(life.hp > life.maxHp)
        life.hp = life.maxHp;

      hp = life.hp;
    }

    if(entitySystem.has_component<Component::Magic>(entity) == true)
    {
      auto& magic = entitySystem.get_component<Component::Magic>(entity);
      if(magic.mp != magic.maxMp)
      {
        int32_t amount = (int32_t)std::ceil(magic.maxMp * 0.02);
        amount = amount * stanceModifier;
        //TODO: update amount based on equipment values
        //TODO: Take into account MP regen buffs
        magic.mp += amount;

        if(magic.mp > magic.maxMp)
          magic.mp = magic.maxMp;

        mp = magic.mp;
      }
    }
    auto p = SrvSetHpAndMp::create(basicInfo.id, hp, mp);
    entitySystem.send_nearby(entity, p);
  }

  // Check if there is damage queued
  if(entitySystem.has_component<Component::Combat>(entity) == true)
  {
    auto& queuedDamage = entitySystem.get_component<Component::Combat>(entity);
    int32_t adjusted_hp = life.hp;
    uint32_t total_applied_damage = 0;

    for(auto& attack : queuedDamage.damage_)
    {
      // We waited at least one update before applying damage, apply it now
      Entity attacker = entitySystem.get_entity_from_id(attack.attacker_);
      logger->debug("Applying damage to entity '{}' {}", basicInfo.name, basicInfo.id);

      if(adjusted_hp <= 0) {
        logger->debug("Entity '{}' {} is dead already, not applying damage", basicInfo.name, basicInfo.id);
        //attack.value_ = 0;
        continue;
      }

      if((adjusted_hp - attack.value_) <= 0) {
        logger->debug("Entity '{}' {} will die from {} damage", basicInfo.name, basicInfo.id, attack.value_);
        total_applied_damage = attack.value_ + adjusted_hp;
        adjusted_hp = 0;
        //TODO: Credit this attacker as the one who killed this entity.
      } else {
        total_applied_damage += attack.value_;
        adjusted_hp -= attack.value_;
      }

      if(adjusted_hp <= 0) {
        //TODO: Get dropped item data here and send it with the DAMAGE packet
        Combat::drop_loot(entitySystem, entity, attacker);
        attack.action_ &= ~DAMAGE_ACTION_HIT;
        attack.action_ |= DAMAGE_ACTION_DEAD;
        auto p = SrvDamage::create(attack.attacker_, basicInfo.id, attack.value_, attack.action_);
        entitySystem.send_nearby(entity, p);
        break;
      } else {
        logger->debug("applied {} damage to entity '{}' {}.", attack.value_, basicInfo.name, basicInfo.id);
        auto p = SrvDamage::create(attack.attacker_, basicInfo.id, attack.value_, attack.action_);
        entitySystem.send_to(entity, p);
        entitySystem.send_to(attacker, p);
      }

      // Add this damage to the combat log
      bool found_log = false;
      for(auto& attack_log : queuedDamage.damage_log_)
      {
        if(attack_log.attacker_ == attack.attacker_)
        {
          attack_log.value_ += attack.value_;
          found_log = true;
          break;
        }
      }

      if(found_log == false)
      {
        queuedDamage.damage_log_.push_back(attack);
      }
    }
    life.hp = adjusted_hp;
    queuedDamage.damage_.clear();

    if(life.hp <= 0) {
      if(entitySystem.has_component<Component::Mob>(entity) == true)
      {
        auto& npcLua = entitySystem.get_component<Component::NpcLua>(entity);
        auto data = npcLua.data.lock();
        int32_t give_exp = data->get_give_exp();
        int32_t xp_out = 1;
        int32_t level_difference = 0;
        int32_t server_rate = 100; // this is the default for a 1x server
        for(auto& attack_log : queuedDamage.damage_log_)
        {
          Entity attacker = entitySystem.get_entity_from_id(attack_log.attacker_);
          auto& attackerLevel = entitySystem.get_component<Component::Level>(attacker);

          // Start exp out algo
          level_difference = attackerLevel.level - level.level;
          if(attack_log.value_ > (life.maxHp * 1.15f))
            attack_log.value_ = (life.maxHp * 1.15f);

          float dmgModifier = (attack_log.value_ + life.maxHp / 15.0f + 30);
          auto expCalc = ((level.level + 3) * give_exp * dmgModifier);

          if(level_difference <= 3)
            xp_out = expCalc * server_rate / life.maxHp / 370.0f;
          else if(level_difference < 9)
            xp_out = expCalc * server_rate / life.maxHp / (level_difference + 3) / 60.0f;
          else
            xp_out = expCalc * server_rate / life.maxHp / (level_difference + 3) / 180.0f;

          if(xp_out < 1)
            xp_out = 1;

          attackerLevel.xp += xp_out;
          // End exp out algo

          Combat::check_for_level_up(entitySystem, attacker, basicInfo.id);

          xp_out = 1;
          level_difference = 0;
        }

        entitySystem.add_timer(5s, [entity](EntitySystem& entitySystem) { entitySystem.delete_entity(entity); });
      }

      // remove components that we can't have if we are dead!
      entitySystem.remove_component<Component::Combat>(entity);
      entitySystem.remove_component<Component::Target>(entity);
      entitySystem.remove_component<Component::Destination>(entity);
    }
  }

  if(values.combatDt > 0)
    values.combatDt -= dt;

  // Check to see if we have a target component
  if(entitySystem.has_component<Component::Target>(entity) == true)
  {
    auto& target = entitySystem.get_component<Component::Target>(entity);
    //const auto& targetBasicInfo = entitySystem.get_component<Component::BasicInfo>(target.target);

    if(((entitySystem.has_component<Component::Npc>(target.target) == false && entitySystem.has_component<Component::Mob>(target.target) == true) ||
      (false))) // TODO:: Check if this map has PVP turned on and the target player isn't on my team
    {
      const auto& targetLife = entitySystem.get_component<Component::Life>(target.target);
      // Are we in attack range?
      if(targetLife.hp > 0)
      {
        if(get_range_to(entitySystem, entity, target.target) <= values.attackRange && values.combatDt <= 0)
        {
          values.combatDt = 1000 - (250 * (values.atkSpeed * 0.01f));
          if(entitySystem.has_component<Component::Combat>(target.target) == false) {
            entitySystem.add_component<Component::Combat>(target.target);
          }

          logger->debug("Attack speed is {}.", values.atkSpeed);
          logger->debug("queuing damage to target entity");
          auto& damage = entitySystem.get_component<Component::Combat>(target.target);

          auto action = 0;
          auto dmg_value = Calculations::get_damage(entitySystem, entity, target.target, 1);
          if(dmg_value > 0) action |= DAMAGE_ACTION_HIT;
          damage.addDamage(basicInfo.id, action, dmg_value);
        }
      }
      else
      {
        entitySystem.remove_component<Component::Target>(entity);
      }
    }
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

std::tuple<uint16_t, float, float> Combat::get_spawn_point(EntitySystem& entitySystem, Entity entity, int type) {
  auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
  const auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(entity);
  const auto& pos = entitySystem.get_component<Component::Position>(entity);

  uint16_t map_id = pos.map;
  float x = 520000.f, y = 520000.f;

  switch(type)
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
      //logger->warn("Combat::get_spawn_point player {} sent a revive type that doesn't exist...", basicInfo.name);
      break;
    }
  }

  return {map_id, x, y};
}

void Combat::revive(EntitySystem& entitySystem, Entity entity, const RoseCommon::Packet::CliReviveReq& packet) {
  auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
  logger->trace("Combat::revive");

  auto& life = entitySystem.get_component<Component::Life>(entity);
  auto& magic = entitySystem.get_component<Component::Magic>(entity);

  auto dest = get_spawn_point(entitySystem, entity, packet.get_reviveType());
  if(life.hp <= 0)
  {
    life.hp = (life.maxHp * 0.3f);
    magic.mp = 0;
  }

  entitySystem.teleport_entity(entity, std::get<1>(dest), std::get<2>(dest), std::get<0>(dest));
}

int64_t Combat::get_exp_to_level(int level) {
  if ( level > 210 )
    level = 210;

  if ( level <= 15 )
      return (int64_t)((level + 3) * (level) * (level + 10) * 0.7);

  if ( level <= 50 )
    return (int64_t)((level - 5) * (level + 2) * (level + 2) * 2.2);

  if ( level <= 100 )
    return (int64_t)((level - 5) * (level + 2) * (level - 38) * 9);

  if ( level <= 139 )
    return (int64_t)((level + 27) * (level + 34) * (level + 220));

  return (int64_t)((level - 15) * (level + 7) * (level - 126) * 41);
}

void Combat::drop_loot(EntitySystem& entitySystem, RoseCommon::Entity entity, RoseCommon::Entity owner) {
  // TODO: compute drop
  const auto& lua_component = entitySystem.get_component<Component::NpcLua>(entity);
  const auto& lua_data = lua_component.data.lock();
  if (!lua_data) {
    // TODO: log?
    return; // no data, no drop!
  }
  if (Core::Random::getInstance().get_uniform(0, 100) <= lua_data->get_drop_money()) {
    const auto& pos = entitySystem.get_component<Component::Position>(entity);
    auto [posX, posY] = Core::Random::getInstance().random_in_circle(pos.x, pos.y, 200.f); //TODO: change that value??
    const int64_t amount = 100; // TODO change that value
    const float deviation = 0.05f;
    Items::drop_item(entitySystem, entitySystem.create_zuly(Core::Random::getInstance().get_normal(amount, deviation)), posX, posY, owner);
  }
}

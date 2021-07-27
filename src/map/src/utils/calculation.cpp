#include "utils/calculation.h"
#include "entity_system.h"
#include "utils/getters.h"

#include "components/basic_info.h"
#include "components/client.h"
#include "components/computed_values.h"
#include "components/inventory.h"
#include "components/level.h"
#include "components/life.h"
#include "components/lua.h"
#include "components/magic.h"
#include "components/position.h"
#include "components/skills.h"
#include "components/stamina.h"
#include "components/stats.h"
#include "random.h"
#include "dataconsts.h"

using namespace RoseCommon;

namespace Calculations {
float get_runspeed(EntitySystem& entitySystem, Entity entity) {
  // const auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(entity);
  const auto& stats = entitySystem.get_component<Component::Stats>(entity);
  auto& values = entitySystem.get_component<Component::ComputedValues>(entity);
  float moveSpeed = BASE_MOVE_SPEED;

  if (values.moveMode <= MoveMode::RUN) {
    int itemSpeed = 65, itemNumber = 0;

    if (entitySystem.has_component<Component::Inventory>(entity)) {
      auto& inventory = entitySystem.get_component<Component::Inventory>(entity);
      const auto& boots = inventory.boots();
      if (boots != entt::null) itemSpeed = Utils::get_move_speed(entitySystem, boots);

      const auto& backpack = inventory.backpack();
      itemSpeed += Utils::get_move_speed(entitySystem, backpack);
    }
    itemSpeed += 20;

    moveSpeed +=
        itemSpeed * (stats.dex + 500.f) / 100.f + 0;  // TODO: Change 0 to value calcd from buffs for movement speed
    // passiveSpeed = passiveSkillValue[MoveSpeed] + moveSpeed * passiveSkillRate[MoveSpeed] / 100.f
    float passiveSpeed = 0 + moveSpeed * 0 / 100.f;
    moveSpeed += passiveSpeed;
  } else {
    // We are riding a cart/gear

    moveSpeed = 200;

    moveSpeed += 0;  // Get buffs value for movement speed

  }
  if (get_weight(entitySystem, entity) >= 100 && moveSpeed > 300) moveSpeed = WALK_SPEED;
  values.runSpeed = moveSpeed;
  return moveSpeed;
}

float get_attackspeed(EntitySystem& entitySystem, Entity entity) {
  const auto& stats = entitySystem.get_component<Component::Stats>(entity);
  auto& values = entitySystem.get_component<Component::ComputedValues>(entity);
  float weaponAtkSpd = 8, passiveAtkSpeed = 0;

  if (entitySystem.has_component<Component::Inventory>(entity)) {
    auto& inventory = entitySystem.get_component<Component::Inventory>(entity);
    const auto& weapon_r = inventory.weapon_r();
    if (weapon_r != entt::null) weaponAtkSpd = Utils::get_attack_speed(entitySystem, weapon_r);
  }
  // Note: passiveAtkSpeed is only set depending on your job?
  float attackSpeed = (passiveAtkSpeed + 1500.f / (weaponAtkSpd + 5));
  // TODO: Do buff processing here
  // attackSpeed + attack speed buff - attack speed debuff
  if (attackSpeed < 30) attackSpeed = 30;

  values.atkSpeed = attackSpeed;
  return attackSpeed;
}

int get_weight(EntitySystem& entitySystem, Entity entity) { return 0; }

int get_maxweight(EntitySystem& entitySystem, Entity entity) { return 0; }

int get_maxhp(EntitySystem& entitySystem, Entity entity) {
  auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(entity);
  auto& level = entitySystem.get_component<Component::Level>(entity);
  auto& life = entitySystem.get_component<Component::Life>(entity);
  auto& stats = entitySystem.get_component<Component::Stats>(entity);

  //TODO:: Move this calc to lua?
  int levelAddition, levelModifier;
  float clampValue;
  switch (basicInfo.job) {
    case CLASS_SOLDIER_111:
      levelAddition = 5, levelModifier = 20, clampValue = 3.5f;
      break;
    case CLASS_SOLDIER_121:
      levelAddition = 5, levelModifier = 28, clampValue = 3.5f;
      break;
    case CLASS_SOLDIER_122:
      levelAddition = 5, levelModifier = 22, clampValue = 3.5f;
      break;
    case CLASS_MAGICIAN_211:
      levelAddition = 4, levelModifier = 26, clampValue = 2.36f;
      break;
    case CLASS_MAGICIAN_221:
      levelAddition = 5, levelModifier = 26, clampValue = 2.37f;
      break;
    case CLASS_MAGICIAN_222:
      levelAddition = 7, levelModifier = 26, clampValue = 2.4f;
      break;
    case CLASS_MIXER_311:
      levelAddition = 5, levelModifier = 20, clampValue = 2.7f;
      break;
    case CLASS_MIXER_321:
      levelAddition = 5, levelModifier = 23, clampValue = 3.f;
      break;
    case CLASS_MIXER_322:
      levelAddition = 5, levelModifier = 21, clampValue = 2.7f;
      break;
    case CLASS_MERCHANT_411:
    case CLASS_MERCHANT_421:
    case CLASS_MERCHANT_422:
      levelAddition = 5, levelModifier = 20, clampValue = 2.7f;
      break;

    default:
      levelAddition = 4, levelModifier = 26, clampValue = 2.36f;
      break;
  }

  life.maxHp = (short)((level.level + levelAddition) * sqrtf(level.level + levelModifier) * clampValue + (stats.str * 2) +
                       0); // AddValue[AT_MAX_HP]);
  // levelAddition = passiveSkillValue[ maxHP ] + (short)(life.maxHp * passiveSkillRate[maxHp] / 100.f);
  life.maxHp += levelAddition;
  return life.maxHp;
}

int get_maxmp(EntitySystem& entitySystem, Entity entity) {
  auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(entity);
  auto& level = entitySystem.get_component<Component::Level>(entity);
  auto& magic = entitySystem.get_component<Component::Magic>(entity);
  auto& stats = entitySystem.get_component<Component::Stats>(entity);

  //TODO:: Move this calc to lua?
  int addValue, statModifier;
  float levelModifier;
  switch (basicInfo.job) {
    case CLASS_SOLDIER_111:
      addValue = 3, levelModifier = 4.0f, statModifier = 4;
      break;
    case CLASS_SOLDIER_121:
      addValue = 0, levelModifier = 4.5f, statModifier = 4;
      break;
    case CLASS_SOLDIER_122:
      addValue = -6, levelModifier = 5.0f, statModifier = 4;
      break;
    case CLASS_MAGICIAN_211:
      addValue = 0, levelModifier = 6.0f, statModifier = 4;
      break;
    case CLASS_MAGICIAN_221:
      addValue = -7, levelModifier = 7.0f, statModifier = 4;
      break;
    case CLASS_MAGICIAN_222:
      addValue = -4, levelModifier = 6.5f, statModifier = 4;
      break;
    case CLASS_MIXER_311:
    case CLASS_MIXER_321:
      addValue = 4, levelModifier = 4.0f, statModifier = 4;
      break;
    case CLASS_MIXER_322:
      addValue = 0, levelModifier = 4.5f, statModifier = 4;
      break;
    case CLASS_MERCHANT_411:
    case CLASS_MERCHANT_421:
      addValue = 3, levelModifier = 4.0f, statModifier = 4;
      break;
    case CLASS_MERCHANT_422:
      addValue = 0, levelModifier = 4.5f, statModifier = 4;
      break;
    default:
      addValue = 4, levelModifier = 3.0f, statModifier = 4;
      break;
  }

  magic.maxMp = (short)((level.level + addValue) * levelModifier + (stats.int_ * statModifier)) + 0;  // AddValue[AT_MAX_MP];
  // addValue = passiveSkillValue[ maxMP ] + (short)(magic.maxMp * passiveSkillRate[maxMp] / 100.f);
  magic.maxMp += addValue;
  return magic.maxMp;
}

int get_successrate(EntitySystem& entitySystem, Entity attacker, Entity defender) {
  int success_rate = 0;
  int final_success_rate = 100;  // TODO: set this to 0 once the calc is finished
//  auto& attackerStats = entitySystem.get_component<Component::Stats>(attacker);
//  auto& defenderStats = entitySystem.get_component<Component::Stats>(defender);

  if (entitySystem.has_component<Component::Client>(attacker)) {
    if (entitySystem.has_component<Component::Client>(defender)) {
      auto random = 1 + Core::Random::getInstance().get_uniform(0, 100);
      // final_success_rate = (int)(90 - ((attacker_hit + defender_avoidance) / attacker_hit) * 40.f + random);
    } else {
      const auto& attackerLevel = entitySystem.get_component<Component::Level>(attacker);
      const auto& defenderLevel = entitySystem.get_component<Component::Level>(defender);
      auto random1 = 1 + Core::Random::getInstance().get_uniform(0, 50);
      auto random2 = 1 + Core::Random::getInstance().get_uniform(0, 60);
      success_rate = (int)((attackerLevel.level + 10) - defenderLevel.level * 1.1f + random1);
      if (success_rate <= 0) return 0;

      // final_success_rate =  (int)(success_rate * (attacker_hit * 1.1f - defender_avoidance * 0.93f + random2
      // + 5 + attackerLevel.level * 0.2f) / 80.f);
    }
  } else {
    auto random = 1 + Core::Random::getInstance().get_uniform(0, 100);
    // final_success_rate = 138 - ((float)(attacker_hit + defender_avoidance) / attacker_hit) * 75.0f + random;
  }
  return final_success_rate;
}

int64_t get_magicdamage(EntitySystem& entitySystem, Entity attacker, Entity defender,
                        int hit_count, int success_rate) {
  // TODO
  int magic_value = 0;
  if (entitySystem.has_component<Component::Inventory>(attacker)) {
    auto& inventory = entitySystem.get_component<Component::Inventory>(attacker);
    const auto& weapon_r = inventory.weapon_r();
    if (weapon_r != entt::null) magic_value = Utils::get_magic(entitySystem, weapon_r);
  }

  return 50;
}

int64_t get_basicdamage(EntitySystem& entitySystem, Entity attacker, Entity defender,
                        int hit_count, int success_rate) {
  int64_t damage = 0, critChance = 0;
  const auto& attackerLevel = entitySystem.get_component<Component::Level>(attacker);
  const auto& values = entitySystem.get_component<Component::ComputedValues>(attacker);

  critChance = (int)(28 - ((values.critChance / 2.0f + attackerLevel.level) / (attackerLevel.level + 8)) * 20) + 1 +
               Core::Random::getInstance().get_uniform(0, 100);

  return 50;
}

int64_t get_damage(EntitySystem& entitySystem, Entity attacker, Entity defender,
                   int hit_count) {
  auto successRate = get_successrate(entitySystem, attacker, defender);

  if (successRate < 20) {
    const auto& attackerLevel = entitySystem.get_component<Component::Level>(attacker);
    const auto& defenderLevel = entitySystem.get_component<Component::Level>(defender);

    if (94 >
        (int)(1 + Core::Random::getInstance().get_uniform(0, 100) + (attackerLevel.level - defenderLevel.level) * 0.6f))
      return 0;
  }

  int magicStat = 0;
  if (entitySystem.has_component<Component::Inventory>(attacker)) {
    auto& inventory = entitySystem.get_component<Component::Inventory>(attacker);
    const auto& weapon_r = inventory.weapon_r();
    if (weapon_r != entt::null) magicStat = Utils::get_magic(entitySystem, weapon_r);
  }

  const auto& values = entitySystem.get_component<Component::ComputedValues>(attacker);
  if (magicStat > 0) return get_magicdamage(entitySystem, attacker, defender, hit_count, successRate);

  return get_basicdamage(entitySystem, attacker, defender, hit_count, successRate);
}
}  // namespace Calculations

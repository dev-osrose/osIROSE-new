#include "utils/calculation.h"
#include "entity_system.h"

#include "components/basic_info.h"
#include "components/computed_values.h"
#include "components/level.h"
#include "components/life.h"
#include "components/magic.h"
#include "components/stamina.h"
#include "components/lua.h"
#include "components/position.h"
#include "components/skills.h"
#include "components/stats.h"
#include "components/inventory.h"

using namespace RoseCommon;

namespace Calculations {
  float get_runspeed(EntitySystem& entitySystem, RoseCommon::Entity entity) {
    //const auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(entity);
    const auto& stats = entitySystem.get_component<Component::Stats>(entity);
    auto& values = entitySystem.get_component<Component::ComputedValues>(entity);
    float moveSpeed = 0;

    if (values.moveMode <= MoveMode::RUN) {
      int itemSpeed = 65, itemNumber = 0;

      if(entitySystem.has_component<Component::Inventory>(entity) == true) {
        auto& inventory = entitySystem.get_component<Component::Inventory>(entity);
        const auto& boots = inventory.boots();
        if(boots) {
          if(entitySystem.has_component<Component::ItemLua>(boots) == true) {
            auto& lua = entitySystem.get_component<Component::ItemLua>(boots);
            if(auto api = lua.api.lock(); api)
              itemSpeed = api->get_move_speed(); // Override the default item speed
          }
        }

        const auto& backpack = inventory.backpack();
        if(backpack) {
          if(entitySystem.has_component<Component::ItemLua>(backpack) == true) {
            auto& lua = entitySystem.get_component<Component::ItemLua>(backpack);
            if(auto api = lua.api.lock(); api)
              itemSpeed += api->get_move_speed();
          }
        }
      }
      itemSpeed += 20;

      moveSpeed = itemSpeed * (stats.dex + 500.f) / 100.f + 0; // TODO: Change 0 to value calcd from buffs for movement speed
      //passiveSpeed = passiveSkillValueMoveSpeed + moveSpeed * passiveSkillRateMoveSpeed / 100.f
      float passiveSpeed = 0 + moveSpeed * 0 / 100.f;
      moveSpeed += passiveSpeed;
    } else {
      // We are riding a cart/gear

      moveSpeed = 200;

      moveSpeed += 0;  // Get buffs value for movement speed

      //if (get_weight(entitySystem, entity) >= 100 && moveSpeed > 300) moveSpeed = 300;
    }
    values.runSpeed = moveSpeed;
    return moveSpeed;
  }

  float get_attackspeed(EntitySystem& entitySystem, RoseCommon::Entity entity) {
    const auto& stats = entitySystem.get_component<Component::Stats>(entity);
    auto& values = entitySystem.get_component<Component::ComputedValues>(entity);
    float weaponAtkSpd = 8, passiveAtkSpeed = 0;

    if(entitySystem.has_component<Component::Inventory>(entity) == true) {
      auto& inventory = entitySystem.get_component<Component::Inventory>(entity);
      const auto& weapon_r = inventory.weapon_r();
      if(weapon_r) {
        if(entitySystem.has_component<Component::ItemLua>(weapon_r) == true) {
          auto& lua = entitySystem.get_component<Component::ItemLua>(weapon_r);
          if(auto api = lua.api.lock(); api)
            weaponAtkSpd = api->get_attack_speed();
        }
      }
    }
    //Note: passiveAtkSpeed is only set depending on your job?
    float attackSpeed = (passiveAtkSpeed + 1500.f / (weaponAtkSpd + 5));
    //TODO: Do buff processing here
    //attackSpeed + attack speed buff - attack speed debuff
    if(attackSpeed < 30)
      attackSpeed = 30;

    values.atkSpeed = attackSpeed;
    return attackSpeed;
  }

  int get_weight(EntitySystem& entitySystem, RoseCommon::Entity entity) {
    return 0;
  }

  int get_maxhp(EntitySystem& entitySystem, RoseCommon::Entity entity) {
    auto& life = entitySystem.get_component<Component::Life>(entity);
    return 0;
  }

  int get_maxmp(EntitySystem& entitySystem, RoseCommon::Entity entity) {
    auto& magic = entitySystem.get_component<Component::Magic>(entity);
    return 0;
  }
}
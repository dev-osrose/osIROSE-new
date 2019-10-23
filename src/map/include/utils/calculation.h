#pragma once

#include <string>
#include <optional>
#include "dataconsts.h"

class EntitySystem;

namespace Calculations {
  float get_runspeed(EntitySystem&, RoseCommon::Entity);
  float get_attackspeed(EntitySystem&, RoseCommon::Entity);

  int get_weight(EntitySystem&, RoseCommon::Entity);

  int get_maxhp(EntitySystem&, RoseCommon::Entity);
  int get_maxmp(EntitySystem&, RoseCommon::Entity);

  int get_successrate(EntitySystem& entitySystem, RoseCommon::Entity attacker, RoseCommon::Entity defender);
  int64_t get_magicdamage(EntitySystem& entitySystem, RoseCommon::Entity attacker, RoseCommon::Entity defender, int hit_count, int success_rate);
  int64_t get_basicdamage(EntitySystem& entitySystem, RoseCommon::Entity attacker, RoseCommon::Entity defender, int hit_count, int success_rate);
  int64_t get_damage(EntitySystem& entitySystem, RoseCommon::Entity attacker, RoseCommon::Entity defender, int hit_count);
}

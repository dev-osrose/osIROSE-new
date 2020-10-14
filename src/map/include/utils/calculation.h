#pragma once

#include <optional>
#include <string>
#include "entity.h"

class EntitySystem;

namespace Calculations {
float get_runspeed(EntitySystem&, Entity);
float get_attackspeed(EntitySystem&, Entity);

int get_weight(EntitySystem&, Entity);
int get_maxweight(EntitySystem&, Entity);

int get_maxhp(EntitySystem&, Entity);
int get_maxmp(EntitySystem&, Entity);

int get_successrate(EntitySystem& entitySystem, Entity attacker, Entity defender);
int64_t get_magicdamage(EntitySystem& entitySystem, Entity attacker, Entity defender,
                        int hit_count, int success_rate);
int64_t get_basicdamage(EntitySystem& entitySystem, Entity attacker, Entity defender,
                        int hit_count, int success_rate);
int64_t get_damage(EntitySystem& entitySystem, Entity attacker, Entity defender, int hit_count);
}  // namespace Calculations

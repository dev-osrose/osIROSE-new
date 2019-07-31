#pragma once

#include "entity_system.h"

namespace Utils {
int  get_attack_speed(EntitySystem& entitySystem, RoseCommon::Entity entity);
int  get_move_speed(EntitySystem& entitySystem, RoseCommon::Entity entity);
int  get_magic(EntitySystem& entitySystem, RoseCommon::Entity entity);
}

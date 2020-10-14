#pragma once

#include "entity_system.h"

namespace Utils {
int  get_attack_speed(EntitySystem& entitySystem, Entity item);
int  get_move_speed(EntitySystem& entitySystem, Entity item);
int  get_magic(EntitySystem& entitySystem, Entity item);
}

#pragma once

#include "entity.h"
#include "entity_system.h"

namespace Utils {
template <typename T>
void transfer_to_char(EntitySystem& entitySystem, Entity entity, const T& packet) {
    entitySystem.send_to(entity, packet);
}
}

#pragma once

#include <vector>
#include "dataconsts.h"
#include "entity_system.h"
#include "components/basic_info.h"

namespace Utils {
template <typename T>
void transfer_to_char_server(EntitySystem& entitySystem, RoseCommon::Entity entity, const T& packet) {
    const auto& basic = entitySystem.get_component<Component::BasicInfo>(entity);
    entitySystem.send_to_char_server(packet, basic.charId);
}
}

#pragma once

#include <vector>
#include "dataconsts.h"
#include "entity_system.h"
#include "components/basic_info.h"

namespace Utils {
template <typename T>
void transfer_to_char_server(EntitySystem& entitySystem, RoseCommon::Entity, const T& packet) {
    const auto& basic = entitySystem.get_component<RoseCommon::Component::BasicInfo>(entity);
    entitySystem.transfer_to_char_server(packet, basic.charId);
}
}

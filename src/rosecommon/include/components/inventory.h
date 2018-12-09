#pragma once

#include "dataconsts.h"
#include <array>
#include <entt.hpp>

namespace Component {
struct Inventory {
    int64_t zuly;
    std::array<RoseCommon::Entity, RoseCommon::EquippedPosition::MAX_EQUIP_ITEMS> equippedItems;
    std::array<RoseCommon::Entity, RoseCommon::BulletType::MAX_BULLET_TYPES> bullets;
    std::array<RoseCommon::Entity, RoseCommon::RidingItem::MAX_RIDING_ITEMS> ridingItems;
    std::array<RoseCommon::Entity, RoseCommon::MAX_ITEMS> items;
};
}

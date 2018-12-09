#pragma once

#include "dataconsts.h"
#include <array>
#include <entt.hpp>

namespace Component {
struct Inventory {
    int64_t zuly;
    std::array<RoseCommon::Entity, RoseCommon::EquippedPosition::MAX_EQUIP_ITEMS> equippedItems = { entt::null };
    std::array<RoseCommon::Entity, RoseCommon::BulletType::MAX_BULLET_TYPES> bullets = { entt::null };
    std::array<RoseCommon::Entity, RoseCommon::RidingItem::MAX_RIDING_ITEMS> ridingItems = { entt::null };
    std::array<RoseCommon::Entity, RoseCommon::MAX_ITEMS> items = {entt::null };
};
}

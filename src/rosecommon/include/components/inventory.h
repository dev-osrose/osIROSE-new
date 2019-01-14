#pragma once

#include "array_wrapper.h"
#include "dataconsts.h"
#include <array>
#include <entt.hpp>

#include <type_traits>

namespace Component {
struct Inventory {
    Inventory() {
        items.fill(entt::null);
    }
    int64_t zuly;
    std::array<RoseCommon::Entity, RoseCommon::MAX_ITEMS> items;

    template <typename T, size_t N, size_t L>
    using base_wrapper = Core::array_wrapper<T, N, L>;
    template <size_t N, size_t L>
    using wrapper = base_wrapper<std::array<RoseCommon::Entity, RoseCommon::MAX_ITEMS>, N, L>;
    template <size_t N, size_t L>
    using const_wrapper = base_wrapper<std::add_const_t<std::array<RoseCommon::Entity, RoseCommon::MAX_ITEMS>>, N, L>;

    constexpr wrapper<0, RoseCommon::EquippedPosition::MAX_EQUIP_ITEMS> getEquipped() {
        return { items };
    }

    constexpr wrapper<0, RoseCommon::MAX_VISIBLE_ITEMS> getVisible() {
        return { items };
    }

    constexpr wrapper<RoseCommon::EquippedPosition::MAX_EQUIP_ITEMS + RoseCommon::MAX_INVENTORY, RoseCommon::MAX_BULLET_TYPES> getBullets() {
        return { items };
    }

    constexpr wrapper<RoseCommon::EquippedPosition::MAX_EQUIP_ITEMS + RoseCommon::MAX_INVENTORY + RoseCommon::MAX_BULLET_TYPES, RoseCommon::MAX_RIDING_ITEMS> getRidingItems() {
        return { items };
    }

    constexpr const_wrapper<0, RoseCommon::EquippedPosition::MAX_EQUIP_ITEMS> getEquipped() const {
        return { items };
    }

    constexpr const_wrapper<0, RoseCommon::MAX_VISIBLE_ITEMS> getVisible() const {
        return { items };
    }

    constexpr const_wrapper<RoseCommon::EquippedPosition::MAX_EQUIP_ITEMS + RoseCommon::MAX_INVENTORY, RoseCommon::MAX_BULLET_TYPES> getBullets() const {
        return { items };
    }

    constexpr const_wrapper<RoseCommon::EquippedPosition::MAX_EQUIP_ITEMS + RoseCommon::MAX_INVENTORY + RoseCommon::MAX_BULLET_TYPES, RoseCommon::MAX_RIDING_ITEMS> getRidingItems() const {
        return { items };
    }
};
}

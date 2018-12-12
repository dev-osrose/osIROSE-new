#pragma once

#include "dataconsts.h"
#include <array>
#include <entt.hpp>

namespace {
template <typename T>
struct array_wrapper { T& iterable; const size_t start; const size_t length; };

template <typename T>
auto begin(array_wrapper<T> w) {
    return std::begin(w.iterable) + w.start;
}

template <typename T>
auto end(array_wrapper<T> w) {
    return std::begin(w.iterable) + w.start + w.length;
}
}

namespace Component {
struct Inventory {
    int64_t zuly;
    std::array<RoseCommon::Entity, RoseCommon::MAX_ITEMS> items = {entt::null };

    using wrapper = array_wrapper<std::array<RoseCommon::Entity, RoseCommon::MAX_ITEMS>>;

    constexpr wrapper getEquipped() {
        return { items, 1, RoseCommon::EquippedPosition::MAX_EQUIP_ITEMS };
    }

    constexpr wrapper getVisible() {
        return { items, 1, RoseCommon::EquippedPosition::MAX_VISIBLE_ITEMS };
    }

    constexpr wrapper getBullets() {
        return { items, RoseCommon::EquippedPosition::MAX_EQUIP_ITEMS + RoseCommon::MAX_INVENTORY, RoseCommon::MAX_BULLET_TYPES };
    }

    constexpr wrapper getRidingItems() {
        return { items, RoseCommon::EquippedPosition::MAX_EQUIP_ITEMS + RoseCommon::MAX_INVENTORY + RoseCommon::MAX_BULLET_TYPES, RoseCommon::MAX_RIDING_ITEMS };
    }
};
}

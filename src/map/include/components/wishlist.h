#pragma once

#include "dataconsts.h"
#include "entity.h"
#include <array>
#include <entt.hpp>

namespace Component {
struct Wishlist {
    Wishlist() {
        items.fill(entt::null);
    }
    std::array<Entity, RoseCommon::MAX_WISHLIST> items;
};
}

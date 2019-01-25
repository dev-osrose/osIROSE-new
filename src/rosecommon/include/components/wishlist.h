#pragma once

#include "dataconsts.h"
#include <array>
#include <entt.hpp>

namespace Component {
struct Wishlist {
    Wishlist() {
        items.fill(entt::null);
    }
    std::array<RoseCommon::Entity, RoseCommon::MAX_WISHLIST> items;
};
}

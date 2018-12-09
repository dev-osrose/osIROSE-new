#pragma once

#include "dataconsts.h"
#include <array>
#include <entt.hpp>

namespace Component {
struct Wishlist {
    std::array<RoseCommon::Entity, RoseCommon::MAX_WISHLIST> items = { entt::null };
};
}

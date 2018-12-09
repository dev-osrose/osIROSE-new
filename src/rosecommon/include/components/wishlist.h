#pragma once

#include "dataconsts.h"
#include <array>

namespace Component {
struct Whishlist {
    std::array<RoseCommon::Entity, RoseCommon::MAX_WISHLIST> items;
};
}

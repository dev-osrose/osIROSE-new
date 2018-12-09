#pragma once

#include "dataconsts.h"
#include <array>

namespace Component {
struct Hotbar {
    std::array<RoseCommon::HotbarItem, RoseCommon::MAX_HOTBAR_ITEMS> items;
};
}

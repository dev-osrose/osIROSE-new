#pragma once

#include "item.h"
#include <array>

struct Inventory {
    static const uint16_t maxItems = 140; // 120 items + equipped + bullets + ride

    std::array<RoseCommon::Item, maxItems> items_;
};


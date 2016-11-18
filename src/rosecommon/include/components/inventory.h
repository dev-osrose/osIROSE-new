#pragma once

#include "components/item.h"
#include <array>

struct Inventory {
    static const uint16_t maxItems = 140; // 120 items + equipped + bullets + ride

    Inventory() {}

    std::array<Item, maxItems> items_;
};


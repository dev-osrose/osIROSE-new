#pragma once

#include "components/item.h"
#include <array>
#include <initializer_list>

struct RidingItems {
    enum RidingPosition {
        BODY = 0,
        ENGINE = 1,
        LEGS,
        //OPTION, // weapon or back seat
        ARMS,
        MAX_RIDING_ITEMS
    };

    RidingItems() {}
    RidingItems(const std::initializer_list<Item> &items) {
        int64_t diff = MAX_RIDING_ITEMS - items.size();
        diff = diff < 0 ? 0 : diff;
        auto last = items.end() - diff;
        std::copy(items.begin(), last, items_.begin());
    }

    std::array<Item, MAX_RIDING_ITEMS> items_;
};


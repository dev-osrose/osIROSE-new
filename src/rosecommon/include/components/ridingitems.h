#pragma once

#include "item.h"
#include <array>

struct RidingItems {
    enum RidingPosition {
        BODY = 0,
        ENGINE = 1,
        LEGS,
        //OPTION, // weapon or back seat
        ARMS,
        MAX_RIDING_ITEMS
    };

    std::array<RoseCommon::Item, MAX_RIDING_ITEMS> items_;
};


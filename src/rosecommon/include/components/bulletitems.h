#pragma once

#include <array>
#include "item.h"

struct BulletItems {
    enum BulletType {
        ARROW = 0,
        BULLET = 1,
        THROW = 2,
        MAX_BULLET_TYPES
    };

    std::array<RoseCommon::Item, MAX_BULLET_TYPES> items_;
};


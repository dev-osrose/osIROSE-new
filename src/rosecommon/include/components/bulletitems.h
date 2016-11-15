#pragma once

#include "components/item.h"
#include <array>
#include <initializer_list>

struct BulletItems {
    enum BulletType {
        ARROW = 0,
        BULLET = 1,
        THROW = 2,
        MAX_BULLET_TYPES
    };

    BulletItems() {}
    BulletItems(const std::initializer_list<Item> &items) {
        int64_t diff = MAX_BULLET_TYPES - items.size();
        diff = diff < 0 ? 0 : diff;
        auto last = items.end() - diff;
        std::copy(items.begin(), last, items_.begin());
    }

    std::array<Item, MAX_BULLET_TYPES> items_;
};


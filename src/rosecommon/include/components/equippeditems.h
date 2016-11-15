#pragma once

#include "components/item.h"
#include <array>
#include <initializer_list>

struct EquippedItems {
    enum EquippedPosition {
        FACE = 0,
        HAIR = 1,
        HELMET,
        ARMOR,
        GAUNTLET,
        BOOTS,
        GOOGLES,
        BACKPACK,
        WEAPON_R,
        WEAPON_L,
        MAX_EQUIPPED_ITEMS
    };

    EquippedItems() {}
    EquippedItems(const std::initializer_list<Item> &items) {
        int64_t diff = MAX_EQUIPPED_ITEMS - items.size();
        diff = diff < 0 ? 0 : diff;
        auto last = items.end() - diff;
        std::copy(items.begin(), last, items_.begin());
    }

   std::array<Item, MAX_EQUIPPED_ITEMS> items_;
};


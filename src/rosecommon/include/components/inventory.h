#pragma once

#include "item.h"
#include <array>

struct Inventory {
    enum Position {
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

    static const uint16_t maxItems = 140; // 120 items + equipped + bullets + ride

    std::array<RoseCommon::Item, maxItems> items_;

    std::array<RoseCommon::Item, MAX_EQUIPPED_ITEMS> getEquipped() const {
        std::array<RoseCommon::Item, MAX_EQUIPPED_ITEMS> data;
        for (size_t i = 0; i < MAX_EQUIPPED_ITEMS; ++i)
            data[i] = items_[i];
        return data;
    }
};


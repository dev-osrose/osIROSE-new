#pragma once

#include "item.h"
#include <array>

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

   std::array<RoseCommon::Item, MAX_EQUIPPED_ITEMS> items_;
};


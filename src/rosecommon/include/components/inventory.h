#pragma once

#include "item.h"
#include <array>

struct Inventory {
    enum Position {
	GOOGLES = 0,
        HELMET = 1,
        ARMOR,
        GAUNTLET,
        BOOTS,
        BACKPACK,
	JEWEL,
        WEAPON_R,
        WEAPON_L,
        MAX_EQUIPPED_ITEMS
    };

    static const uint16_t maxItems = 140; // 120 items + equipped + bullets + ride

    std::array<RoseCommon::Item, maxItems> items_;

    std::array<RoseCommon::Item, MAX_EQUIPPED_ITEMS> getEquipped() const {
        std::array<RoseCommon::Item, MAX_EQUIPPED_ITEMS> data;
	data[0] = items_[HELMET];
	data[1] = items_[ARMOR];
	data[2] = items_[GAUNTLET];
	data[3] = items_[BOOTS];
	data[4] = items_[GOOGLES];
	data[5] = items_[BACKPACK];
	data[6] = items_[JEWEL];
	data[7] = items_[WEAPON_R];
	data[8] = items_[WEAPON_L];
        return data;
    }
};


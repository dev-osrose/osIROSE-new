#pragma once

#include <array>
#include "item.h"

template <typename T>
struct array_wrapper { const T& iterable; const size_t start; const size_t length; const typename T::value_type& operator[](size_t index) const { return *(std::begin(iterable) + start + index); } };

template <typename T>
auto begin(array_wrapper<T> w) {
    return std::begin(w.iterable) + w.start;
}

template <typename T>
auto end(array_wrapper<T> w) {
    return std::begin(w.iterable) + w.start + w.length;
}

struct Inventory {
    enum Position {
        GOGGLES = 1,
        HELMET = 2,
        ARMOR,
        BACKPACK,
        GAUNTLET,
        BOOTS,
        WEAPON_R,
        WEAPON_L,
        NECKLACE,
        RING,
        EARING,
        MAX_EQUIP_ITEMS
    };

    static constexpr uint16_t maxItems = 140; // 120 items + equipped + bullets + ride
    static constexpr uint8_t maxVisibleEquippedItems = 8;

    template <typename T, typename U>
    void loadFromResult(T& res, U& builder) {
        for (const auto &row : res) {
            if (row.slot >= maxItems)
                continue; // FIXME : error somewhere?
            items_[row.slot].loadFromRow(row, builder);
        }
    }

    using equipped_wrapper = array_wrapper<const std::array<RoseCommon::Item, maxItems>>;

    std::array<RoseCommon::Item, maxItems> items_{};

    equipped_wrapper getEquipped() const {
        return { items_, 1, maxVisibleEquippedItems };
    }
};

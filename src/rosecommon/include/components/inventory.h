#pragma once

#include "item.h"
#include <array>

template <typename T>
struct array_wrapper { T& iterable; const size_t start; const size_t length; };

template <typename T>
auto begin(array_wrapper<T> w) {
  return std::begin( w.iterable ) + w.start;
}

template <typename T>
auto end(array_wrapper<T> w) {
  return std::begin( w.iterable ) + w.start + w.length;
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

  static const uint16_t maxItems = 140; // 120 items + equipped + bullets + ride

  using equipped_wrapper = array_wrapper<const std::array<RoseCommon::Item, maxItems>>;

  std::array<RoseCommon::Item, maxItems> items_;

  equipped_wrapper getEquipped() const {
    static const uint8_t maxVisibleEquippedItems = 8;
    return { items_, 1, maxVisibleEquippedItems };
  }
};

#pragma once

#include "item.h"

#include <array>

struct Wishlist {
  static const uint8_t maxWishes = 30;

  std::array<RoseCommon::Item, maxWishes> wishlist_;

  friend
  RoseCommon::CRosePacket &operator<<(RoseCommon::CRosePacket &os, const Wishlist& w) {
    for (auto &it : w.wishlist_)
      os << it.getHeader() << it.getData();
    return os;
  }

  template <typename T, typename U>
  void loadFromResult(T& res, U& builder) {
    for (const auto& row : res) {
      if (row.slot >= maxWishes)
        continue; // FIXME : error somewhere?
      wishlist_[row.slot].loadFromRow(row, builder);
    }
  }
};

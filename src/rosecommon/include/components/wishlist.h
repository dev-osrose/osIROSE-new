#pragma once

#include "item.h"

#include <array>
#include "iserialize.h"

struct Wishlist : public RoseCommon::ISerialize {
  static const uint8_t maxWishes = 30;

  std::array<RoseCommon::Item, maxWishes> wishlist_;

  virtual bool read(RoseCommon::CRoseReader& reader) override {
    for (auto& wish : wishlist_)
        if (!reader.get_iserialize(wish)) return false;
    return true;
  }

  virtual bool write(RoseCommon::CRoseBasePolicy& writer) const override {
      for (const auto& wish : wishlist_)
          if (!writer.set_iserialize(wish)) return false;
      return true;
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

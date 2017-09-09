#pragma once

#include "item.h"

#include <array>

struct Wishlist {
  static const maxWishes = 30;

  std::array<RoseCommon::Item, maxWishes> wishlist_;
};

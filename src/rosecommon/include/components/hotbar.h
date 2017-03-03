#pragma once

#include <array>
#include <initializer_list>
#include "crosepacket.h"

// This is not a component!
struct HotbarItem {
  HotbarItem() : item_( 0 ) {}
  HotbarItem(uint16_t item) : item_( item ) {}

  HotbarItem(uint8_t type, uint16_t slotId) : type_( type ),
                                              slotId_( slotId ) {}

  virtual ~HotbarItem() {}

  union {
    uint16_t item_;

    struct {
      uint8_t type_ : 5;
      uint16_t slotId_ : 11;
    };
  };
};

struct Hotbar {
  static const uint16_t maxItems = 32;

  std::array<HotbarItem, maxItems> items_;
};

#pragma once

namespace Component {
  struct Item {
    bool isCreated;
    bool is_zuly;
    uint16_t life;
    uint8_t durability;
    bool hasSocket;
    bool isAppraised;
    uint8_t refine;
    uint32_t count;
    uint16_t gemOpt;
    uint32_t price;
  };
}

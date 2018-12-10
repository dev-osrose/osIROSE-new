#pragma once

namespace Component {
struct Item {
    uint8_t type;
    uint16_t id;
    bool isCreated;
    uint16_t life;
    bool hasSocket;
    bool isAppraised;
    uint8_t refine;
    uint32_t count;
    uint16_t gemOpt;
};
}

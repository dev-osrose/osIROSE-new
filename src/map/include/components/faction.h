#pragma once

#include <array>

namespace Component {
struct Faction {
    uint8_t id;
    uint8_t rank;
    uint8_t fame;
    std::array<uint16_t, 2> factionFame;
    std::array<uint16_t, 10> points = {0};
};
}

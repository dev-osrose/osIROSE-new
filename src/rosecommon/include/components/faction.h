#pragma once

#include <array>

namespace Component {
struct Faction {
    uint8_t factionId;
    uint8_t factionRank;
    uint8_t fame;
    std::array<uint16_t, 2> factionFame;
    std::array<uint16_t, 10> factionPoints;
};
}

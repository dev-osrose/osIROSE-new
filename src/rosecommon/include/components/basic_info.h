#pragma once

#include <string>

namespace Component {
struct BasicInfo {
    std::string name; // name of character
    uint16_t id; // unique ID on the map
    uint32_t tag; // unique ID among all server
    int32_t teamId; // entities with same teamId cannot attack each other (for pvp, summons)
};
}

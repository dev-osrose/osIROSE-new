#pragma once

#include <string>

namespace Component {
struct BasicInfo {
    std::string name; // name of character
    uint16_t id; // unique ID on the map
    uint32_t tag; // unique ID among all server
    int32_t teamId; // entities with same teamId cannot attack each other (for pvp, summons)
    uint16_t job;
    uint32_t statPoints;
    uint32_t skillPoints;
    uint16_t pkFlag;
    uint8_t stone;
    uint32_t charId;
};
}

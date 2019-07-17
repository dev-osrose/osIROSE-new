#pragma once

#include "dataconsts.h"
#include <vector>

namespace RoseCommon {
struct PartyBase {
    PartyBase(RoseCommon::Entity leader);

    bool add_member(RoseCommon::Entity);
    bool remove_member(uint32_t tag);

    bool is_member(uint32_t tag) const;

    bool change_leader(uint32_t tag);

    uint32_t xp = 0;
    int range = 700;
    uint8_t level = 1;
    uint8_t options = 0;
    uint32_t leader = 0;
    RoseCommon::Entity last_item = entt::null;
    RoseCommon::Entity last_etc = entt::null;
    RoseCommon::Entity last_zuly = entt::null;
    uint8_t capacity = 5;
    std::vector<uint32_t> members;
};
}

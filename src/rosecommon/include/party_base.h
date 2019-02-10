#pragma once

#include "dataconsts.h"
#include <vector>

namespace RoseCommon {
struct PartyBase {
    PartyBase(RoseCommon::Entity leader);

    bool add_member(RoseCommon::Entity);
    bool remove_member(RoseCommon::Entity);

    bool is_member(RoseCommon::Entity) const;

    bool change_leader(RoseCommon::Entity);

    uint32_t xp = 0;
    int range = 700;
    uint8_t level = 1;
    uint8_t options = 0;
    RoseCommon::Entity leader = entt::null;
    RoseCommon::Entity last_item = entt::null;
    RoseCommon::Entity last_etc = entt::null;
    RoseCommon::Entity last_zuly = entt::null;
    uint8_t capacity = 5;
    std::vector<RoseCommon::Entity> members;
};
}

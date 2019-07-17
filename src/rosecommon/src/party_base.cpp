#include "party_base.h"
#include <algorithm>

namespace RoseCommon {
PartyBase::PartyBase(uint32_t leader) : leader(leader) {
    members.reserve(capacity);
    members.push_back(leader);
}

bool PartyBase::add_member(uint32_t tag) {
    if (tag == 0) {
        return false;
    }
    if (std::find(members.begin(), members.end(), tag) != members.end()) {
        return false;
    }
    members.push_back(tag);
    return true;
}

bool PartyBase::remove_member(uint32_t tag) {
    const auto pos = std::find(members.begin(), members.end(), tag);
    if (pos == members.end()) {
        return false;
    }
    members.erase(pos);
    if (leader == tag && members.size() != 0) {
        leader = members[0];
    } else if (leader == tag) {
        leader = 0;
    }
    return true;
}

bool PartyBase::is_member(uint32_t en) const {
    return std::find(members.begin(), members.end(), en) != members.end();
}

bool PartyBase::change_leader(uint32_t en) {
    if (!is_member(en) || leader == en) {
        return false;
    }
    leader = en;
    return true;
}
}

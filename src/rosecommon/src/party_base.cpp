#include "party_base.h"
#include <algorithm>

namespace RoseCommon {
PartyBase::PartyBase(Entity leader) : leader(leader) {
    members.reserve(capacity);
    members.push_back(leader);
}

bool PartyBase::add_member(Entity member) {
    if (member == entt::null) {
        return false;
    }
    if (std::find(members.begin(), members.end(), member) != members.end()) {
        return false;
    }
    members.push_back(member);
    return true;
}

bool PartyBase::remove_member(RoseCommon::Entity member) {
    const auto pos = std::find(members.begin(), members.end(), member);
    if (pos == members.end()) {
        return false;
    }
    members.erase(pos);
    if (leader == member && members.size() != 0) {
        leader = members[0];
    } else if (leader == member) {
        leader = entt::null;
    }
    return true;
}

bool PartyBase::is_member(RoseCommon::Entity en) const {
    return std::find(members.begin(), members.end(), en) != members.end();
}

bool PartyBase::change_leader(RoseCommon::Entity en) {
    if (!is_member(en) || leader == en) {
        return false;
    }
    leader = en;
    return true;
}
}

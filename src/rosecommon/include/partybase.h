// Copyright 2016 Chirstopher Torres (Raven), L3nn0x
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http ://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*!
 * \file partybase
 * \brief Contains the base party struct
 *
 * \author L3nn0x
 * \date october 2016
 */
#pragma once

#include <vector>
#include <memory>
#include <algorithm>

#include "crosepacket.h"

namespace PartyReply {
enum PartyReply : uint8_t {
    NOT_FOUND = 0,
    BUSY = 1,
    ACCEPT_MAKE,
    ACCEPT_JOIN,
    REJECT,
    DESTROY,
    FULL_MEMBERS,
    INVALID_LEVEL,
    CHANGE_OWNER,
    CHANGE_OWNER_DISCONNECT,
    NO_CHANGE_TARGET,
    KICK = 0x80,
    DISCONNECT,
    REJOIN
};

}

namespace PartyRequest {
enum PartyRequest : uint8_t {
    MAKE = 0,
    JOIN = 1,
    LEFT,
    CHANGE_OWNER,
    KICK = 0x81
};

}

struct PartyBase {
    PartyBase() :
        xp_(0),
        range_(7), // FIXME : set that in a configurable way
        level_(1),
        options_(0),
        lastItem_(-1),
        lastEtc_(-1),
        lastZuly_(-1),
        capacity_(5) {} // FIXME : set that in a configurable way

    PartyBase(Entity leader) : PartyBase() {
        leader_ = leader;
        members_ = {leader};
    }

    bool addMember(Entity member) {
        if (!member)
            return false;
        if (std::find(members_.begin(), members_.end(), member) != members_.end())
            return false;
        members_.push_back(member);
        return true;
    }

    bool removeMember(Entity member) {
        auto pos = std::find(members_.begin(), members_.end(), member);
        if (pos == members_.end())
            return false;
        members_.erase(pos);
        if (leader_ == member)
            leader_ = members_[0];
        return true;
    }

    bool isMember(Entity member) {
        return std::find(members_.begin(), members_.end(), member) != members_.end();
    }

    bool changeLeader(Entity leader) {
        if (!isMember(leader) || leader == leader_)
            return false;
        leader_ = leader;
        return true;
    }

    uint32_t xp_;
    int range_;
    uint8_t level_;
    Entity leader_;
    std::vector<Entity> members_;
    uint8_t options_;
    int lastItem_; // id of last person to receive an item
    int lastEtc_; // id of last person to receive an etc
    int lastZuly_; // id of last person to receive zuly
    uint8_t capacity_;
};

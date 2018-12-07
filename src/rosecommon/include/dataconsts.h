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

#ifndef _DATACONSTS_H_
#define _DATACONSTS_H_

namespace RoseCommon {
constexpr unsigned int MIN_SELL_TYPE = 1;
constexpr unsigned int MAX_SELL_TYPE = 11;

constexpr unsigned int MAX_ITEMS = 140;

constexpr unsigned int MAX_UNION_COUNT = 10;
constexpr unsigned int MAX_BUFF_STATUS = 40;
constexpr unsigned int MAX_SKILL_COUNT = 120;
constexpr unsigned int MAX_HOTBAR_ITEMS = 32;

constexpr unsigned int MAX_DAMAGE = 99999999;

constexpr unsigned int DAMAGE_ACTION_ATTACK = 0x02;
constexpr unsigned int DAMAGE_ACTION_HIT = 0x4;
constexpr unsigned int DAMAGE_ACTION_CRITICAL = 0x8;
constexpr unsigned int DAMAGE_ACTION_DEAD = 0x10;

constexpr unsigned int MAX_CONDITIONS_EPISODE = 5;
constexpr unsigned int MAX_CONDITIONS_JOB = 3;
constexpr unsigned int MAX_CONDITIONS_PLANET = 7;
constexpr unsigned int MAX_CONDITIONS_UNION = 10;
constexpr unsigned int MAX_QUESTS = 10;
constexpr unsigned int MAX_SWITCHES = 16;

constexpr unsigned int MAX_QUEST_SWITCHES = 32;
constexpr unsigned int MAX_QUEST_VARS = 10;
constexpr unsigned int MAX_QUEST_ITEMS = 6;
}

namespace ReviveReq {
    enum ReviveReq : uint8_t {
        REVIVE_POS = 1,
        SAVE_POS,
        START_POST,
        CURRENT_POS
    };
}

namespace PartyReq {
enum Request : uint8_t {
    MAKE = 0,
    JOIN = 1,
    LEFT,
    CHANGE_OWNER,
    KICK = 0x81
};
}

namespace PartyReply {
enum Reply : uint8_t {
    NOT_FOUND = 0,
    BUSY = 1,
    ACCEPT_MAKE,
    ACCEPT_JOIN,
    REJECT,
    DESTROY,
    FULL,
    INVALID_LEVEL,
    CHANGE_OWNER,
    CHANGE_OWNER_DISCONNECT,
    NO_CHANGE_TARGET,
    KICK = 0x80,
    DISCONNECT,
    REJOIN
};
}

/*namespace SetItem {
struct Item : public RoseCommon::ISerialize {
    Item() = default;
    Item(const Item&) = default;
    Item(Item&&) = default;
    Item& operator=(const Item&) = default;
    Item& operator=(Item&&) = default;
    Item(uint8_t id, uint16_t header, uint32_t data) : id_(id), header_(header), data_(data) {}
    virtual ~Item() = default;

    virtual bool write(RoseCommon::CRoseBasePolicy& writer) const override {
        if (!writer.set_uint8_t(id_)) return false;
        if (!writer.set_uint16_t(header_)) return false;
        return writer.set_uint32_t(data_);
    }

    virtual bool read(RoseCommon::CRoseReader& reader) override {
        if (!reader.get_uint8_t(id_)) return false;
        if (!reader.get_uint16_t(header_)) return false;
        return reader.get_uint32_t(data_);
    }

    uint8_t id_;
    uint16_t header_;
    uint32_t data_;
};
}*/

namespace ServerData {
enum Type : uint8_t {
    ECONOMY = 0,
    NPC
};
}

#endif

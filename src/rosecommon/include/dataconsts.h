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

#include <string>
#include "iserialize.h"
#include "crosewriter.h"

namespace RoseCommon {
#define MIN_SELL_TYPE     1
#define MAX_SELL_TYPE     11

#define MAX_UNION_COUNT   10
#define MAX_BUFF_STATUS   40
#define MAX_SKILL_COUNT   120
#define MAX_HOTBAR_ITEMS  32
#define MAX_DAMAGE        99999999

#define DAMAGE_ACTION_ATTACK    0x02
#define DAMAGE_ACTION_HIT       0x04
#define DAMAGE_ACTION_CRITICAL  0x08
#define DAMAGE_ACTION_DEAD      0x10
}

namespace ReviveReq {
    enum ReviveReq : uint8_t {
        REVIVE_POS = 1,
        SAVE_POS,
        START_POST,
        CURRENT_POS
    };
}

namespace CharListReply {
enum equipped_position {
    EQUIP_HELMET = 0,
    EQUIP_ARMOR = 1,
    EQUIP_GAUNTLET,
    EQUIP_BOOTS,
    EQUIP_GOGGLES,
    EQUIP_BACKPACK,
    EQUIP_WEAPON_R,
    EQUIP_WEAPON_L,
    MAX_EQUIPPED_ITEMS
};

constexpr equipped_position getPosition(uint8_t pos) {
    return static_cast<equipped_position>(pos);
}

struct equip_item {
    union {
        PACK(struct {
            unsigned int id_ : 10;     // 0~1023
            unsigned int gem_op_ : 9;  // 0~512
            unsigned int socket_ : 1;  // 0~1
            unsigned int grade_ : 4;   // 0~15
        });
        uint32_t data = 0;
    };
};

struct CharInfo : public RoseCommon::ISerialize {
    std::string name_;
    uint8_t race_;
    uint16_t level_;
    uint16_t job_;
    uint32_t face_;
    uint32_t hair_;
    uint32_t remain_sec_until_delete_;
    uint8_t platinium_;
    equip_item items_[MAX_EQUIPPED_ITEMS];

    virtual ~CharInfo() = default;
    CharInfo() = default;
    CharInfo(const std::string name, uint8_t race, uint16_t level, uint16_t job, uint32_t face, uint32_t hair, uint32_t secsToDelete, uint8_t platinium = 0) :
        name_(name), race_(race), level_(level), job_(job), face_(face), hair_(hair), remain_sec_until_delete_(secsToDelete), platinium_(platinium) {}

    void addEquipItem(equipped_position slot, uint16_t id, uint16_t gem = 0, uint8_t socket = 0, uint8_t grade = 0) {
        if (slot >= equipped_position::MAX_EQUIPPED_ITEMS) return;
        items_[slot].id_ = id;
        items_[slot].gem_op_ = gem;
        items_[slot].socket_ = socket;
        items_[slot].grade_ = grade;
    }

    bool read(RoseCommon::CRoseReader& reader) override {
        if (!reader.get_string(name_)) return false;
        if (!reader.get_uint8_t(race_)) return false;
        if (!reader.get_uint16_t(level_)) return false;
        if (!reader.get_uint16_t(job_)) return false;
        if (!reader.get_uint32_t(remain_sec_until_delete_)) return false;
        if (!reader.get_uint8_t(platinium_)) return false;
        if (!reader.get_uint32_t(face_)) return false;
        if (!reader.get_uint32_t(hair_)) return false;

        for (size_t i = 0; i < MAX_EQUIPPED_ITEMS; ++i) {
            if (!reader.get_uint32_t(items_[i].data)) return false;
        }
        return true;
    }

    bool write(RoseCommon::CRoseBasePolicy& writer) const override {
        if (!writer.set_string(name_)) return false;
        if (!writer.set_uint8_t(race_)) return false;
        if (!writer.set_uint16_t(level_)) return false;
        if (!writer.set_uint16_t(job_)) return false;
        if (!writer.set_uint32_t(remain_sec_until_delete_)) return false;
        if (!writer.set_uint8_t(platinium_)) return false;
        if (!writer.set_uint32_t(face_)) return false;
        if (!writer.set_uint32_t(hair_)) return false;

        for (const auto& item : items_) {
            if (!writer.set_uint32_t(item.data)) return false;
        }
        return true;
    }
};
}

namespace CreateCharReply {
enum Result : uint8_t {
    OK = 0,
    FAILED,
    NAME_TAKEN,
    INVALID_VALUE,
    TOO_MANY_CHARS,
    BLOCKED
};
}

namespace SelectCharReply {
enum Result : uint8_t {
    OK = 0,
    FAILED,
    FULL,
    INVALID_CHANNEL,
    CHANNEL_NOT_ACTIVE,
    INVALID_AGE
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

namespace JoinServerReply {
enum Result : uint8_t {
    OK = 0,
    FAILED,
    TIME_OUT,
    INVALID_PASSWORD,
    ALREADY_LOGGEDIN
};
}

namespace SetItem {
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
}

namespace ServerData {
enum Type : uint8_t {
    ECONOMY = 0,
    NPC
};
}

/*namespace CharacterListReply {
struct Character : public RoseCommon::ISerialize {
    Character(std::string name, uint8_t race, uint16_t level, uint8_t job, uint32_t deleteSeconds, uint8_t platinium, uint32_t face, uint32_t hair) :
        name_(name), deleteSeconds_(deleteSeconds), level_(level), race_(race), job_(job), platinium_(platinium), face_(face), hair_(hair) {}

    std::string name_;
    uint32_t deleteSeconds_;
    uint16_t level_;
    uint8_t race_;
    uint16_t job_;
    uint8_t platinium_;
    uint32_t face_;
    uint32_t hair_;
    uint32_t items_[Inventory::Position::MAX_EQUIP_ITEMS - 2] = {0}; // minus face and hair

    void addEquipItem(uint8_t slot, uint16_t itemId, uint16_t gem, uint8_t socket, uint8_t grade) {
        if (slot >= Inventory::Position::MAX_EQUIP_ITEMS - 2) return;
        items_[slot] = itemId << 14 | gem << 5 | socket << 4 | grade;
    }

    virtual bool write(RoseCommon::CRoseBasePolicy& writer) const override {
        if (!writer.set_string(name_)) return false;
        if (!writer.set_uint8_t(race_)) return false;
        if (!writer.set_uint16_t(level_)) return false;
        if (!writer.set_uint16_t(job_)) return false;
        if (!writer.set_uint32_t(deleteSeconds_)) return false;
        if (!writer.set_uint8_t(platinium_)) return false;
        if (!writer.set_uint32_t(face_)) return false;
        if (!writer.set_uint32_t(hair_)) return false;
        for (size_t i = 0; i < Inventory::Position::MAX_EQUIP_ITEMS - 2; ++i)
            if (!writer.set_uint32_t(items_[i])) return false;
        return true;
    }

    virtual bool read(RoseCommon::CRoseReader& reader) override {
        if (!reader.get_string(name_)) return false;
        if (!reader.get_uint8_t(race_)) return false;
        if (!reader.get_uint16_t(level_)) return false;
        if (!reader.get_uint16_t(job_)) return false;
        if (!reader.get_uint32_t(deleteSeconds_)) return false;
        if (!reader.get_uint8_t(platinium_)) return false;
        if (!reader.get_uint32_t(face_)) return false;
        if (!reader.get_uint32_t(hair_)) return false;
        for (size_t i = 0; i < Inventory::Position::MAX_EQUIP_ITEMS - 2; ++i)
            if (!reader.get_uint32_t(items_[i])) return false;
        return true;
    }
};
}*/

#endif

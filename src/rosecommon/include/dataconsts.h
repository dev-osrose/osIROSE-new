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

#include "iserialize.h"
#include <chrono>
#include <entt.hpp>

namespace RoseCommon {
namespace {
using namespace std::chrono_literals;
}
constexpr unsigned int MIN_SELL_TYPE = 1;
constexpr unsigned int MAX_SELL_TYPE = 11;

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

using Entity = uint32_t;
using Registry = entt::registry<Entity>;

enum BulletType : uint8_t {
    ARROW = 0,
    BULLET = 1,
    THROW = 2,
    MAX_BULLET_TYPES
};

enum RidingItem : uint8_t {
    BODY = 0,
    ENGINE = 1,
    LEGS,
    //OPTION, // weapon or back seat
    ARMS,
    MAX_RIDING_ITEMS
};

enum EquippedPosition : uint8_t {
    GOGGLES = 1,
    HELMET = 2,
    ARMOR,
    BACKPACK,
    GAUNTLET,
    BOOTS,
    WEAPON_R,
    WEAPON_L,
    NECKLACE,
    RING,
    EARING,
    MAX_EQUIP_ITEMS
};

constexpr unsigned int MAX_INVENTORY = 120;

constexpr unsigned int MAX_ITEMS = MAX_INVENTORY + BulletType::MAX_BULLET_TYPES + RidingItem::MAX_RIDING_ITEMS + EquippedPosition::MAX_EQUIP_ITEMS;
static_assert(MAX_ITEMS != 140, "The client expects 140 total items");

enum MoveMode : uint8_t {
    WALK = 0,
    RUN = 1,
    DRIVE = 2,
    RIDEON = 4
};

enum Command : uint16_t {
    STOP = 0,
    MOVE = 1,
    ATTACK = 2,
    DIE = 3,
    PICKUP = 4,
    SKILL2SELF = 6,
    SKILL2OBJ = 7,
    SKILL2POS = 8,
    RUNAWAY = 0x8009,
    SIT = 10
};

struct HotbarItem : public ISerialize {
    virtual bool read(CRoseReader& reader) override {
        if (!reader.get_uint16_t(data.item)) {
            return false;
        }
        return true;
    }

    virtual bool write(CRoseBasePolicy& writer) const override {
        if (!writer.set_uint16_t(data.item)) {
            return false;
        }
        return true;
    }
    
    static constexpr size_t size() { return sizeof(data); }
    
    void set_type(uint8_t type) { data.type = type; }
    uint8_t get_type() const { return data.type; };
    void set_slotId(uint16_t id) { data.slotId = id; }
    uint16_t get_slotId() const { return data.slotId; }
    
    private:
        union {
            PACK(struct {
                uint8_t type : 5;
                uint16_t slotId : 11;
            });
            uint16_t item = 0;
        } data;
};

constexpr unsigned int MAX_SKILLS = 120;

struct Skill : public ISerialize {
    virtual bool read(CRoseReader& reader) override {
        if (!reader.get_uint16_t(id)) {
            return false;
        }
        if (!reader.get_uint8_t(level)) {
            return false;
        }
        return true;
    }

    virtual bool write(CRoseBasePolicy& writer) const override {
        if (!writer.set_uint16_t(id)) {
            return false;
        }
        if (!writer.set_uint8_t(level)) {
            return false;
        }
        return true;
    }
    
    static constexpr size_t size() { return sizeof(id) + sizeof(level); }
    
    void set_id(uint16_t id) { this->id = id; }
    uint16_t get_id() const { return id; }
    void set_level(uint8_t level) { this->level = level; }
    uint8_t get_level() const { return level; }
    
    private:
        uint16_t id = 0;
        uint8_t level = 0;
};

constexpr unsigned int MAX_STATUS_EFFECTS = 40;

struct StatusEffect : public ISerialize {
    virtual bool read(CRoseReader& reader) override {
        uint32_t count;
        if (!reader.get_uint32_t(count)) {
            return false;
        }
        expired = std::chrono::seconds(count);
        if (!reader.get_uint16_t(value)) {
            return false;
        }
        if (!reader.get_uint16_t(unkown)) {
            return false;
        }
        return true;
    }

    virtual bool write(CRoseBasePolicy& writer) const override {
        if (!writer.set_uint32_t(expired.count())) {
            return false;
        }
        if (!writer.set_uint16_t(value)) {
            return false;
        }
        if (!writer.set_uint16_t(unkown)) {
            return false;
        }
        return true;
    }
    
    static constexpr size_t size() { return sizeof(uint32_t) + sizeof(value) + sizeof(unkown); }

    void set_expired(std::chrono::seconds expired) { this->expired = expired; }
    std::chrono::seconds get_expired() const { return expired; }
    void set_value(uint16_t value) { this->value = value; }
    uint16_t get_value() const { return value; }
    void set_unkown(uint16_t unkown) { this->unkown = unkown; }
    uint16_t get_unkown() const { return unkown; }
    void set_dt(std::chrono::milliseconds dt) { this->dt = dt; }
    std::chrono::milliseconds get_dt() const { return dt; }
    
    private:
        std::chrono::seconds expired = 0s;
        uint16_t value = 0;
        uint16_t unkown = 0;
        std::chrono::milliseconds dt = 0ms;
};

constexpr unsigned int MAX_WISHLIST = 30;

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

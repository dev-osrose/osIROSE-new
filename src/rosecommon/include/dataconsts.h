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

#include <entt.hpp>

#include "enums.h"

namespace RoseCommon {
constexpr unsigned int MIN_SELL_TYPE = 1;
constexpr unsigned int MAX_SELL_TYPE = 11;

constexpr unsigned int MAX_STAT = 300; //changed by davidix
constexpr unsigned int MAX_STACK = 999;

constexpr unsigned int MAX_UNION_COUNT = 10;
constexpr unsigned int MAX_BUFF_STATUS = 40;
constexpr unsigned int MAX_SKILL_COUNT = 120;
constexpr unsigned int MAX_HOTBAR_ITEMS = 32;

constexpr unsigned int MAX_DAMAGE = 99999999;

constexpr unsigned int DAMAGE_ACTION_IMMEDIATE = 0x02;
constexpr unsigned int DAMAGE_ACTION_HIT = 0x04;
constexpr unsigned int DAMAGE_ACTION_CRITICAL = 0x08;
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

constexpr float        DROP_RANGE = 50.f;
constexpr unsigned int MAX_VISIBLE_ITEMS = 8;
constexpr unsigned int MAX_INVENTORY = 120;
constexpr unsigned int MAX_ITEMS = MAX_INVENTORY + BulletType::MAX_BULLET_TYPES + RidingItem::MAX_RIDING_ITEMS + EquippedPosition::MAX_EQUIP_ITEMS;
static_assert(MAX_ITEMS == 140, "The client expects 140 total items");

constexpr unsigned int MAX_STATUS_EFFECTS = 40;

constexpr unsigned int MAX_WISHLIST = 30;


//TODO:: Move these class vars to scripting file
constexpr unsigned int CLASS_VISITOR = 0;
constexpr unsigned int CLASS_SOLDIER_111 = 111;
constexpr unsigned int CLASS_SOLDIER_121 = 121;
constexpr unsigned int CLASS_SOLDIER_122 = 122;
constexpr unsigned int CLASS_SOLDIER_131 = 131;
constexpr unsigned int CLASS_SOLDIER_132 = 132;
constexpr unsigned int CLASS_MAGICIAN_211 = 211;
constexpr unsigned int CLASS_MAGICIAN_221 = 221;
constexpr unsigned int CLASS_MAGICIAN_222 = 222;
constexpr unsigned int CLASS_MAGICIAN_231 = 231;
constexpr unsigned int CLASS_MAGICIAN_232 = 232;
constexpr unsigned int CLASS_MIXER_311 = 311;
constexpr unsigned int CLASS_MIXER_321 = 321;
constexpr unsigned int CLASS_MIXER_322 = 322;
constexpr unsigned int CLASS_MIXER_331 = 331;
constexpr unsigned int CLASS_MIXER_332 = 332;
constexpr unsigned int CLASS_MERCHANT_411 = 411;
constexpr unsigned int CLASS_MERCHANT_421 = 421;
constexpr unsigned int CLASS_MERCHANT_422 = 422;
constexpr unsigned int CLASS_MERCHANT_431 = 431;
constexpr unsigned int CLASS_MERCHANT_432 = 432;

using Entity = uint32_t;
using Registry = entt::registry<Entity>;
}

// I place it after just in case we need any of the constexpr/using directives in this file
#include "types.h"

/*namespace PartyReply {
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

namespace ServerData {
enum Type : uint8_t {
    ECONOMY = 0,
    NPC
};
}*/

#endif

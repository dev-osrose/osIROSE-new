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

constexpr unsigned int MAX_STACK = 999;

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
    
constexpr float        DROP_RANGE = 50.f;
constexpr unsigned int MAX_VISIBLE_ITEMS = 8;
constexpr unsigned int MAX_INVENTORY = 120;
constexpr unsigned int MAX_ITEMS = MAX_INVENTORY + BulletType::MAX_BULLET_TYPES + RidingItem::MAX_RIDING_ITEMS + EquippedPosition::MAX_EQUIP_ITEMS;
static_assert(MAX_ITEMS == 140, "The client expects 140 total items");

constexpr unsigned int MAX_STATUS_EFFECTS = 40;

constexpr unsigned int MAX_WISHLIST = 30;

using Entity = uint32_t;
using Registry = entt::registry<Entity>;
}

// I place it after just in case we need any of the constexpr/using directives in this file
#include "types.h"

/*
namespace ServerData {
enum Type : uint8_t {
    ECONOMY = 0,
    NPC
};
}*/

#endif

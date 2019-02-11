#pragma once

namespace RoseCommon {
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
    OPTION, // weapon or back seat
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

enum ItemType : uint8_t {
    NONE = 0,
    ITEM_GOGGLES = 1,
    ITEM_HELMET = 2,
    ITEM_ARMOR = 3,
    ITEM_GAUNTLET = 4,
    ITEM_BOOTS = 5,
    ITEM_BACKPACK = 6,
    ITEM_RING = 7,
    ITEM_WEAPON_R = 8,
    ITEM_WEAPON_L = 9,
    ITEM_CONSUMABLE = 10,
    ITEM_ETC = 12,
    ITEM_ETC2 = 13,
    ITEM_RIDING = 14,
    ZULY = 0x1F
};

}

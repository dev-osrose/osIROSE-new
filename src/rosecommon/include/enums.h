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
    GOGGLE = 1,
    HELMET = 2,
    ARMOR = 3,
    GLAUNTLET = 4,
    BOOTS = 5,
    BACKPACK = 6,
    RING = 7,
    WEAPON_R = 8,
    WEAPON_l = 9,
    CONSO = 10,
    LAST_WEARABLE = CONSO,
    ETC = 12,
    ETC2 = 13,
    RIDING = 14
};

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


#ifndef _ENTITYCOMPONENTS_H_
# define _ENTITYCOMPONENTS_H_

#include <string>
#include "entityx.hh"
#include <array>
#include <vector>
#include <initializer_list>
#include "crosepacket.h"

class CMapClient;

struct SocketConnector {
    SocketConnector(CMapClient *client) : client_(client) {}

    CMapClient *client_;
};

struct BasicInfo {
    enum Cmd {
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

    BasicInfo() : level_(0), xp_(0), id_(0), tag_(0), teamId_(0), targetId_(0), command_(STOP), loggedIn_(false) {}
    BasicInfo(std::string name, uint16_t level, uint32_t xp, uint16_t id, uint32_t tag, int32_t teamId, uint16_t targetId) :
        name_(name), level_(level), xp_(xp), id_(id), tag_(tag), teamId_(teamId), targetId_(targetId) {}

    std::string name_;
    uint16_t level_;
    uint32_t xp_;
    uint16_t id_;
    uint32_t tag_;
    int32_t teamId_;
    uint16_t targetId_;
    uint16_t command_;
    bool loggedIn_;
};

struct Stats {
    Stats() : maxHp_(0), maxMp_(0), str_(0), dex_(0), int_(0), con_(0), charm_(0), sense_(0), bodySize_(0), headSize_(0) {}
    Stats(uint32_t maxHp, uint32_t maxMp, uint16_t str, uint16_t dex,
            uint16_t intel, uint16_t con, uint16_t charm, uint16_t sense, uint8_t bodySize, uint8_t headSize) :
        maxHp_(maxHp), maxMp_(maxMp), str_(str), dex_(dex), int_(intel), con_(con),
        charm_(charm), sense_(sense), bodySize_(bodySize), headSize_(headSize) {}

    uint32_t maxHp_;
    uint32_t maxMp_;
    uint16_t str_;
    uint16_t dex_;
    uint16_t int_;
    uint16_t con_;
    uint16_t charm_;
    uint16_t sense_;
    uint8_t bodySize_;
    uint8_t headSize_;
};

struct AdvancedInfo {
    enum MoveMode {
        WALK = 0,
        RUN = 1,
        DRIVE = 2,
        RIDEON = 4
    };

    AdvancedInfo() : zuly_(0), hp_(0), mp_(0), moveMode_(AdvancedInfo::RUN), runSpeed_(0), atkSpeed_(0), weightRate_(0) {}
    AdvancedInfo(uint32_t zuly, uint32_t hp, uint32_t mp, uint8_t moveMode) : zuly_(zuly), hp_(hp), mp_(mp), moveMode_(moveMode) {}

    uint32_t zuly_;
    uint32_t hp_;
    uint32_t mp_;
    uint8_t moveMode_;
    uint16_t runSpeed_;
    uint16_t atkSpeed_;
    uint8_t weightRate_;
};

struct Graphics {
    Graphics(uint16_t view) : view_(view) {}

    uint16_t view_;
};

struct CharacterGraphics {
    CharacterGraphics() : face_(0), hair_(0), race_(0) {}
    CharacterGraphics(uint8_t face, uint8_t hair, uint8_t race) : face_(face),
        hair_(hair), race_(race) {}

    uint8_t face_;
    uint8_t hair_;
    uint8_t race_;
};

struct CharacterInfo {
    CharacterInfo() :
        job_(0),
        stone_(0),
        statPoints_(0),
        skillPoints_(0),
        penaltyXp_(0),
        deleteDate_(0),
        platinium_(false),
        factionId_(0),
        factionRank_(0),
        factionFame_{{0}},
        factionPoints_{{0}},
        guildId_(0),
        guildContribution_(0),
        guildRank_(0),
        pkFlag_(0),
        statusFlag_(0),
        stamina_(0),
        patHp_(0),
        patCooldownTime_(0),
        dt_(0)
    {}
    CharacterInfo(uint16_t job, uint8_t stone, uint32_t statPoints,
                    uint32_t skillPoints, uint32_t penaltyXp, uint32_t deleteDate, bool platinium,
                    uint8_t faction) :
        job_(job), stone_(stone), statPoints_(statPoints), skillPoints_(skillPoints),
        penaltyXp_(penaltyXp), deleteDate_(deleteDate), platinium_(platinium),
        factionId_(faction) {}

    uint16_t job_;
    uint8_t stone_;
    uint32_t statPoints_;
    uint32_t skillPoints_;
    uint32_t penaltyXp_;
    uint32_t deleteDate_;
    bool platinium_;
    uint8_t factionId_;
    uint8_t factionRank_;
    uint8_t fame_; // no idea what that is
    std::array<uint16_t, 2> factionFame_; // 2 different factions in the game
    std::array<uint16_t, 10> factionPoints_; // crappy packet code, only the first 3 are used
    uint32_t guildId_;
    uint16_t guildContribution_;
    uint8_t guildRank_;
    uint16_t pkFlag_;
    uint32_t statusFlag_;
    uint16_t stamina_;
    uint16_t patHp_;
    uint32_t patCooldownTime_; // in seconds
    double dt_; // to keep track of seconds for patCoolDownTime_
};

struct Position {
    Position() : x_(0), y_(0), z_(0), map_(0), spawn_(0) {}
    Position(float x, float y, uint16_t map, uint16_t spawn) : x_(x), y_(y), z_(0), map_(map), spawn_(spawn) {}

    float x_;
    float y_;
    uint16_t z_;
    uint16_t map_;
    uint16_t spawn_;
};

struct Destination {
    Destination(float x, float y) : x_(x), y_(y) {}

    float x_;
    float y_;
};

// This is not a component!
struct StatusEffect : public RoseCommon::ISerialize {
    StatusEffect() : expiredSeconds_(0), value_(0), unkown_(0), dt_(0) {}
    StatusEffect(uint32_t expiredSeconds, uint16_t value, uint16_t unkown) :
        expiredSeconds_(expiredSeconds), value_(value), unkown_(unkown), dt_(0) {}

    uint32_t expiredSeconds_;
    uint16_t value_;
    uint16_t unkown_;
    double dt_; // to keep track of time (in seconds)

    protected:
        virtual void serialize(RoseCommon::CRosePacket &os) const {
            os << expiredSeconds_ << value_ << unkown_;
        }
        virtual void deserialize(RoseCommon::CRosePacket&) {}
};

struct StatusEffects {
    static const uint16_t maxEffects = 40;

    StatusEffects() : effects_(maxEffects) {}
    StatusEffects(const std::initializer_list<StatusEffect> effects) : effects_(effects) {
        while (effects_.size() > maxEffects)
            effects_.pop_back();
        effects_.resize(maxEffects);
    }

    std::vector<StatusEffect> effects_;
};

// This is not a component!
struct Skill : public RoseCommon::ISerialize {
    Skill() : id_(0), level_(0) {}
    Skill(uint16_t id, uint8_t level) : id_(id), level_(level) {}

    uint16_t id_;
    uint8_t level_;

    protected:
        virtual void serialize(RoseCommon::CRosePacket &os) const {
            os << id_;
        }
        virtual void deserialize(RoseCommon::CRosePacket&) {}

};

struct Skills {
    static const uint16_t maxSkills = 120;

    Skills() {}
    Skills(const std::initializer_list<Skill> &skills) {
        int64_t diff = maxSkills - skills.size();
        diff = diff < 0 ? 0 : diff;
        auto last = skills.end() - diff;
        std::copy(skills.begin(), last, skills_.begin());
    }

    std::array<Skill, maxSkills> skills_;
};

// This is not a component!
struct HotbarItem : public RoseCommon::ISerialize {
    HotbarItem() : item_(0) {}
    HotbarItem(uint16_t item) : item_(item) {}
    HotbarItem(uint8_t type, uint16_t slotId) : type_(type), slotId_(slotId) {}

    union {
        uint16_t item_;
        struct {
            uint8_t type_ : 5;
            uint16_t slotId_ : 11;
        };
    };

    protected:
        virtual void serialize(RoseCommon::CRosePacket &os) const {
            os << item_;
        }
        virtual void deserialize(RoseCommon::CRosePacket&) {}
};

struct Hotbar {
    static const uint16_t maxItems = 32;

    Hotbar() {}
    Hotbar(const std::initializer_list<HotbarItem> &items) {
        int64_t diff = maxItems - items.size();
        diff = diff < 0 ? 0 : diff;
        auto last = items.end() - diff;
        std::copy(items.begin(), last, items_.begin());
    }

    std::array<HotbarItem, maxItems> items_;
};

// This is not a component!
struct Item : public RoseCommon::ISerialize {
    Item() : totalPart1_(0), totalPart2_(0), type_(WEARABLE), runSpeed_(0), atkSpeed_(0) {}

    enum ItemType {
        WEARABLE = 0,
        CONSOMABLE = 1,
        ETC = 2,
        RIDING = 3,
        MAX_INV_TYPE
    };

    union {
        // Wearable
        struct {
            uint8_t type_ : 5;
            uint16_t id_ : 10;
            bool isCreated_;
            uint16_t gemOpt_ : 9;
            uint8_t durability_ : 7;
            uint16_t life_ : 10;
            bool hasSocket_;
            bool isAppraisal_;
            uint8_t refine_ : 4;
        } wearable_;
        // Consomable
        struct {
            uint8_t type_ : 5;
            uint16_t id_ : 10;
            uint32_t quantity_;
        } consomable_;
        // Money
        struct {
            uint8_t type_ : 5;
            uint16_t reserved_ : 11;
            uint32_t zuly_;
        } money_;
        // Bullet
        struct {
            union {
                struct {
                    uint8_t type_ : 5;
                    uint16_t id_ : 10;
                };
                uint16_t item_;
            };
        } bullet_;
        struct {
            uint16_t totalPart1_;
            uint32_t totalPart2_;
        };
    };

    ItemType type_;

    float runSpeed_;
    float atkSpeed_;

    public:
        void partialSerialize(RoseCommon::CRosePacket &os) const {
            uint32_t data = (wearable_.refine_ << 20) | (wearable_.hasSocket_ << 19) |
                            (wearable_.gemOpt_ << 10) | wearable_.id_;
            os << data;
        }
        void bulletPartialSerialize(RoseCommon::CRosePacket &os) const {
            os << bullet_.item_;
        }
    protected:
        virtual void serialize(RoseCommon::CRosePacket &os) const {
            os << totalPart1_ << totalPart2_;
        }
        virtual void deserialize(RoseCommon::CRosePacket&) {}
};

struct EquippedItems {
    enum EquippedPosition {
        FACE = 0,
        HAIR = 1,
        HELMET = 2,
        ARMOR = 3,
        GAUNTLET = 4,
        BOOTS = 5,
        GOGGLES = 6,
        FACE_ITEM = GOGGLES,
        BACKPACK = 7,
        WEAPON_R = 8,
        WEAPON_L = 9,
        MAX_EQUIPPED_ITEMS
    };

    EquippedItems() {}
    EquippedItems(const std::initializer_list<Item> &items) {
        int64_t diff = MAX_EQUIPPED_ITEMS - items.size();
        diff = diff < 0 ? 0 : diff;
        auto last = items.end() - diff;
        std::copy(items.begin(), last, items_.begin());
    }

   std::array<Item, MAX_EQUIPPED_ITEMS> items_;
};

struct RidingItems {
    enum RidingPosition {
        BODY = 0,
        ENGINE = 1,
        LEGS = 2,
        ARMS = 3,
        MAX_RIDING_ITEMS
    };

    RidingItems() {}
    RidingItems(const std::initializer_list<Item> &items) {
        int64_t diff = MAX_RIDING_ITEMS - items.size();
        diff = diff < 0 ? 0 : diff;
        auto last = items.end() - diff;
        std::copy(items.begin(), last, items_.begin());
    }

    std::array<Item, MAX_RIDING_ITEMS> items_;
};

struct BulletItems {
    enum BulletType {
        ARROW = 0,
        BULLET = 1,
        THROW = 2,
        MAX_BULLET_TYPE
    };

    BulletItems() {}

    std::array<Item, MAX_BULLET_TYPE> items_;
};

struct Inventory {
    static const uint16_t maxItems = 138; // 120 items + equipped + bullets + ride

    Inventory() {}

    std::array<Item, maxItems> items_;
};

using GameComponents = entityx::Components<SocketConnector, BasicInfo, Stats, AdvancedInfo,
      CharacterInfo, Graphics, CharacterGraphics, Position, StatusEffects,
      Skills, Hotbar, EquippedItems, Destination, RidingItems, BulletItems, Inventory>;

using EntityManager = entityx::EntityX<GameComponents, entityx::ColumnStorage<GameComponents>>;
template <typename T>
using Component = EntityManager::Component<T>;
using Entity = EntityManager::Entity;

#endif /* !_ENTITYCOMPONENTS_H_ */


#ifndef _ENTITYCOMPONENTS_H_
# define _ENTITYCOMPONENTS_H_

#include <string>
#include "entityx.hh"
#include <array>
#include <vector>
#include <initializer_list>

class CMapClient;

struct SocketConnector {
    SocketConnector(CMapClient *client) : client_(client) {}

    CMapClient *client_;
};

struct BasicInfo {
    BasicInfo(std::string name, uint16_t level, uint32_t xp, uint32_t id) :
        name_(name), level_(level), xp_(xp), id_(id) {}

    std::string name_;
    uint16_t level_;
    uint32_t xp_;
    uint32_t id_;
};

struct Stats {
    Stats(uint32_t maxHp, uint32_t maxMp, uint16_t str, uint16_t dex,
            uint16_t intel, uint16_t con, uint16_t charm, uint16_t sense) :
        maxHp_(maxHp), maxMp_(maxMp), str_(str), dex_(dex), int_(intel), con_(con),
        charm_(charm), sense_(sense) {}

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
    AdvancedInfo(uint32_t zuly, uint32_t hp, uint32_t mp) : zuly_(zuly), hp_(hp), mp_(mp) {}

    uint32_t zuly_;
    uint32_t hp_;
    uint32_t mp_;
};

struct Graphics {
    Graphics(uint16_t view) : view_(view) {}

    uint16_t view_;
};

struct CharacterGraphics {
    CharacterGraphics(uint8_t face, uint8_t hair, uint8_t race) : face_(face),
        hair_(hair), race_(race) {}

    uint8_t face_;
    uint8_t hair_;
    uint8_t race_;
};

struct CharacterInfo {
    CharacterInfo(uint16_t job, uint8_t stone, uint32_t statPoints,
                    uint32_t skillPoints, uint32_t penaltyXp, uint32_t deleteDate, bool platinium,
                    uint8_t un) :
        job_(job), stone_(stone), statPoints_(statPoints), skillPoints_(skillPoints),
        penaltyXp_(penaltyXp), deleteDate_(deleteDate), platinium_(platinium),
        union_(un) {}

    uint16_t job_;
    uint8_t stone_;
    uint32_t statPoints_;
    uint32_t skillPoints_;
    uint32_t penaltyXp_;
    uint32_t deleteDate_;
    bool platinium_;
    uint8_t union_;
    uint8_t rank_;
    uint8_t fame_;
    uint16_t fame1_;
    uint16_t fame2_;
    std::array<uint16_t, 10> unionPoints_; // there are 10 unions in the game
    uint32_t guildId_;
    uint16_t guildContribution_;
    uint8_t guildPosition_;
    uint16_t pkFlag_;
    uint16_t stamina_;
    uint16_t patHp_;
    uint32_t patCooldownTime_;
    double dt_; // to keep track of seconds for patCoolDownTime_
};

struct Position {
    Position(int32_t x, int32_t y, uint16_t map) : x_(x), y_(y), map_(map) {}

    int32_t x_;
    int32_t y_;
    uint16_t map_;
    uint16_t spawn_;
};

struct Destination {
    Destination(int32_t x, int32_t y) : x_(x), y_(y) {}

    int32_t x_;
    int32_t y_;
};

// This is not a component!
struct StatusEffect {
    StatusEffect() : expiredSeconds_(0), value_(0), unkown_(0), dt_(0) {}
    StatusEffect(uint32_t expiredSeconds, uint16_t value, uint16_t unkown) :
        expiredSeconds_(expiredSeconds), value_(value), unkown_(unkown), dt_(0) {}

    uint32_t expiredSeconds_;
    uint16_t value_;
    uint16_t unkown_;
    double dt_; // to keep track of time (in seconds)
};

struct StatusEffects {
    const uint16_t maxEffects = 40;

    StatusEffects() : effects_(maxEffects) {}
    StatusEffects(const std::initializer_list<StatusEffect> effects) : effects_(effects) {
        while (effects_.size() > 40)
            effects_.pop_back();
        effects_.resize(maxEffects);
    }

    std::vector<StatusEffect> effects_;
};

struct Skills {
    Skills() : skills_{0} {}
    Skills(const std::initializer_list<uint16_t> &skills) {
        int64_t diff = 120 - skills.size();
        diff = diff < 0 ? 0 : diff;
        auto last = skills.end() - diff;
        std::copy(skills.begin(), last, skills_.begin());
    }

    std::array<uint16_t, 120> skills_;
};

// This is not a component!
struct HotbarItem {
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
};

struct Hotbar {
    Hotbar() {}
    Hotbar(const std::initializer_list<HotbarItem> &items) {
        int64_t diff = 32 - items.size();
        diff = diff < 0 ? 0 : diff;
        auto last = items.end() - diff;
        std::copy(items.begin(), last, items_.begin());
    }

    std::array<HotbarItem, 32> items_;
};

struct Item {
    Item() : id_(0), gemOpt_(0), hasSocket_(0), grade_(0) {}
    Item(uint16_t id, uint16_t gemOpt, bool hasSocket, uint8_t grade) : id_(id),
        gemOpt_(gemOpt), hasSocket_(hasSocket), grade_(grade) {}

    uint16_t id_ : 10;
    uint16_t gemOpt_ : 9;
    bool hasSocket_;
    uint8_t grade_ : 4;
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

using GameComponents = entityx::Components<SocketConnector, BasicInfo, Stats, AdvancedInfo,
      CharacterInfo, Graphics, CharacterGraphics, Position, Destination, StatusEffects,
      Skills, Hotbar, EquippedItems>;

#endif /* !_ENTITYCOMPONENTS_H_ */

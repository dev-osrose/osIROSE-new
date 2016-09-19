
#ifndef _ENTITYCOMPONENTS_H_
# define _ENTITYCOMPONENTS_H_

#include <string>
#include "entityx.hh"

struct BasicInfo {
    BasicInfo(std::string name, uint16_t level, uint32_t xp, uint32_t charOwner) :
        name_(name), level_(level), xp_(xp), charOwner_(charOwner) {}

    std::string name_;
    uint16_t level_;
    uint32_t xp_;
    uint32_t charOwner_;
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
};

struct AdvancedInfo {
    AdvancedInfo(uint32_t zuly, uint32_t hp, uint32_t mp) : zuly_(zuly), hp_(hp), mp_(mp) {}

    uint32_t zuly_;
    uint32_t hp_;
    uint32_t mp_;
};

struct Graphics {
    Graphics(uint16_t view) : view_(view) {}

    uint16_t view;
};

struct CharacterGraphics {
    CharacterGraphics(uint8_t face, uint8_t hair, uint8_t race) : face_(face),
        hair_(hair), race_(race) {}

    uint8_t face_;
    uint8_t hair_;
    uint8_t race_;
};

struct CharacterInfo {
    CharacterInfo(uint16_t job, uint8_t stone_, uint32_t statPoints,
                    uint32_t skillPoints, uint32_t penaltyXp, uint32_t deleteDate, bool platinium) :
        job_(job), stone_(stone), statPoints_(statPoints), skillPoints_(skillPoints),
        penaltyXp_(penaltyXp), deleteDate_(deleteDate), platinium_(platinium) {}

    uint16_t job_;
    uint8_t stone_;
    uint32_t statPoints_;
    uint32_t skillPoints_;
    uint32_t penaltyXp_;
    uint32_t deleteDate_;
    bool platinium_;
};

struct Position {
    Position(float x, float y, uint16_t map) : x_(x), y_(y), map_(map) {}

    float x_;
    float y_;
    uint16_t map_;
};

struct Destination {
    Destination(float x, float y) : x_(x), y_(y) {}

    float x_;
    float y_;
};

using GameComponents = entityx::Components<BasicInfo, Stats, AdvancedInfo,
      CharacterInfo, Graphics, CharacterGraphics, Position, Destination>;

#endif /* !_ENTITYCOMPONENTS_H_ */

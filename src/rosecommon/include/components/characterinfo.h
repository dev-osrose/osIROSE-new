#pragma once

#include <array>

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
        subFlag_(0),
        stamina_(0),
        patHp_(0),
        patCooldownTime_(0),
        dt_(0)
    {}

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
    uint32_t subFlag_;
    uint16_t stamina_;
    uint16_t patHp_;
    uint32_t patCooldownTime_; // in seconds
    double dt_; // to keep track of seconds for patCoolDownTime_
};


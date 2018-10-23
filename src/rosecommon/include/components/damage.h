#pragma once

#include <array>

// This is not a component!
struct tDamage {
    tDamage() : action_(0), attacker_(0), value_(0) {}
    tDamage(int8_t action, uint32_t attacker, int32_t value) : action_(action), attacker_(attacker), value_(value) {}

    uint8_t action_;
    uint32_t attacker_;
    int32_t value_;
};

struct Damage {
    static const uint16_t maxConcurentDamage = 300;
    
    bool addDamage(uint32_t attacker, uint8_t action, int32_t damage = 0) {
        tDamage newDamage = tDamage(action, attacker, damage);
        damage_.push_back(newDamage);
        return true;
    }
    
    std::vector<tDamage> damage_;
};


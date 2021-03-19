#pragma once
#include <array>

namespace Component {

  // This is not a component! Just the data!
  struct tDamage {
    tDamage() : action_(0), attacker_(0), value_(0), apply_(false) {}
    tDamage(uint32_t action, uint32_t attacker, int32_t value) : action_(action), attacker_(attacker), value_(value), apply_(false) {}

    uint32_t action_;
    uint32_t attacker_;
    int32_t value_;
    bool apply_;
  };

  struct Combat {
    static const uint16_t maxConcurentDamage = 300;

    bool addDamage(uint32_t attacker, uint32_t action, int32_t damage = 0) {
      tDamage newDamage = tDamage(action, attacker, damage);
      damage_.push_back(newDamage);
      return true;
    }

    std::vector<tDamage> damage_;
    std::vector<tDamage> damage_log_;
  };
}

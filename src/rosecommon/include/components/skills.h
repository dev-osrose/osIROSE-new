#pragma once

#include <array>
#include <initializer_list>
#include "crosepacket.h"

// This is not a component!
struct Skill {
  Skill() : id_( 0 ),
            level_( 0 ) {}

  Skill(uint16_t id, uint8_t level) : id_( id ),
                                      level_( level ) {}

  virtual ~Skill() {}

  uint16_t id_;
  uint8_t level_;
};

struct Skills {
  static const uint16_t maxSkills = 120;

  std::array<Skill, maxSkills> skills_;
};

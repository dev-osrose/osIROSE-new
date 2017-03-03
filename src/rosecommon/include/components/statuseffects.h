#pragma once

#include "crosepacket.h"
#include <vector>
#include <initializer_list>

// This is not a component!
struct StatusEffect {
  StatusEffect() : expiredSeconds_( 0 ),
                   value_( 0 ),
                   unkown_( 0 ),
                   dt_( 0 ) {}

  StatusEffect(uint32_t expiredSeconds, uint16_t value, uint16_t unkown = 0) :
    expiredSeconds_( expiredSeconds ),
    value_( value ),
    unkown_( unkown ),
    dt_( 0 ) {}

  virtual ~StatusEffect() {}

  uint32_t expiredSeconds_;
  uint16_t value_;
  uint16_t unkown_;
  double dt_; // to keep track of time (in seconds)
};

struct StatusEffects {
  static const uint16_t maxEffects = 40;

  StatusEffects() : effects_( maxEffects ) {}

  StatusEffects(const std::initializer_list<StatusEffect> effects) : effects_( effects ) {
    while ( effects_.size() > maxEffects )
      effects_.pop_back();
    effects_.resize( maxEffects );
  }

  std::vector<StatusEffect> effects_;
};

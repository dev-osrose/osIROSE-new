#pragma once

#include "dataconsts.h"

namespace Component {
  struct ComputedValues {
    RoseCommon::Command command;
    RoseCommon::MoveMode moveMode;
    uint16_t runSpeed;
    uint16_t atkSpeed;
    uint8_t weightRate;
    uint32_t statusFlag;
    uint32_t subFlag;
    
    // Update time deltaTime values for combat system
    uint32_t combatDt; // Used for combat calcualations
    uint32_t regenDt; // Used for hp/mp regen
  };
}

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
    
    int16_t attackRange = 2;
    
    // Update time deltaTime values for combat system
    int32_t combatDt = 0; // Used for combat calcualations
    uint32_t regenDt = 0; // Used for hp/mp regen
  };
}

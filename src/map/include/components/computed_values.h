#pragma once

#include "dataconsts.h"

namespace Component {
  struct ComputedValues {
    RoseCommon::Command command = RoseCommon::Command::STOP;
    RoseCommon::MoveMode moveMode = RoseCommon::MoveMode::RUN;
    uint16_t runSpeed = RoseCommon::BASE_MOVE_SPEED;
    uint16_t atkSpeed = 30;
    uint8_t weightRate = 0;
    uint32_t statusFlag = 0;
    uint32_t subFlag = 0;
    
    int16_t attackRange = 2;
    int32_t critChance = 1;
    
    // Update time deltaTime values for combat system
    int32_t combatDt = 0; // Used for combat calculations
    uint32_t regenDt = 0; // Used for hp/mp regen
  };
}

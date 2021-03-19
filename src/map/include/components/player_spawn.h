#pragma once

#include "timed_callbacks.h"
#include <vector>
#include "dataconsts.h"
#include <chrono>

namespace Component {
  struct PlayerSpawn {
    enum Type : uint8_t {
    REVIVE_POINT = 0,
    START_POINT,
    RESPAWN_POINT
    };
    
    PlayerSpawn() : type(RESPAWN_POINT) {}
    PlayerSpawn(Type type) : type(type) {}
    
    Type type;
  };
}

#pragma once

struct PlayerSpawn {
    enum Type : uint8_t {
        REVIVE_POINT = 0,
        START_POINT,
        RESPAWN_POINT
    };

    PlayerSpawn() : type_(RESPAWN_POINT) {}
    PlayerSpawn(Type type) : type_(type) {}
    
    Type type_;
};

#pragma once

struct PlayerSpawn {
    enum Type : uint8_t {
        LOGIN_POINT = 0,
        RESPAWN_POINT = 1
    };

    PlayerSpawn() : type_(RESPAWN_POINT) {}
    PlayerSpawn(Type type) : type_(type) {}
    
    Type type_;
};

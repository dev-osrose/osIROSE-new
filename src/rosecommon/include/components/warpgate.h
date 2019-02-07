#pragma once

#include <cmath>

namespace Component {
struct Warpgate {
    float x;
    float y;
    float z;
    float range;
    static constexpr int character_size = 10;

    uint16_t dest_map;

    bool is_point_in(float x, float y, float z) {
        x = x - this->x;
        y = y - this->y;
        z = z - this->z;
        return !(character_size + range < std::sqrt(x * x + y * y + z * z));
    }
};
}

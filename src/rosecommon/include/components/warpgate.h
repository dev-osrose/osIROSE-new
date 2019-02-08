#pragma once

#include <cmath>

namespace Component {
struct Warpgate {
    float min_x;
    float min_y;
    float min_z;
    float max_x;
    float max_y;
    float max_z;
    static constexpr int character_size = 10;

    uint16_t dest_map;

    bool is_point_in(float x, float y, [[maybe_unused]] float z) {
        return x + character_size >= min_x && x - character_size <= max_x
            && y + character_size >= min_y && y - character_size <= max_y;
    }
};
}

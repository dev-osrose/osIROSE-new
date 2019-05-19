#pragma once

#include <cmath>
#include <tuple>

namespace Component {
struct Warpgate {
    int16_t id;

    float min_x;
    float min_y;
    float min_z;
    float max_x;
    float max_y;
    float max_z;
    static constexpr float character_size = 200.f;

    uint16_t dest_map;

    static constexpr inline float squared(float x) { return x * x; }
    
    std::tuple<float, float, float> get_center() const {
        return {(max_x + min_x) / 2.f, (max_y + min_y) / 2.f, (max_z + min_z) / 2.f};
    }
    
    bool is_point_in(float x, float y, [[maybe_unused]] float z) {
        float dist_squared = squared(character_size);
        if (x < min_x) dist_squared -= squared(x - min_x);
        else if (x > max_x) dist_squared -= squared(x - max_x);
        if (y < min_y) dist_squared -= squared(y - min_y);
        else if (y > max_y) dist_squared -= squared(y - max_y);
        //if (z < min_z) dist_squared -= squared(z - min_z);
        //else if (z > max_z) dist_squared -= squared(z - max_z);
        return dist_squared > 0;
    }
};
}

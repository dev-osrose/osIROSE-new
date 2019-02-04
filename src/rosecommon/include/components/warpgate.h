#pragma once

namespace Component {
struct Warpgate {
    static constexpr const float model_min_x = -1.75;
    static constexpr const float model_min_y = -1.75;
    static constexpr const float model_min_z = -0.004103546;
    static constexpr const float model_max_x = 1.75;
    static constexpr const float model_max_y = 1.75;
    static constexpr const float model_max_z = 3.495897;

    float min_x;
    float min_y;
    float min_z;
    float max_x;
    float max_y;
    float max_z;

    uint16_t dest_map;

    bool is_point_in(float x, float y, float z) {
        if (x >= min_x && x <= max_x &&
            y >= min_y && y <= max_y &&
            z >= min_z && z <= max_z) {
            return true;
        }
        return false;
    }
};
}

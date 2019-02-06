#pragma once

namespace Component {
struct Warpgate {
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

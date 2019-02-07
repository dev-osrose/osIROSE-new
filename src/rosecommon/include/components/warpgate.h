#pragma once

namespace Component {
struct Warpgate {
    float min_x;
    float min_y;
    float min_z;
    float max_x;
    float max_y;
    float max_z;
    static constexpr int range = 100;

    uint16_t dest_map;

    bool is_point_in(float x, float y, [[maybe_unused]] float z) {
        // we don't test z as we don't have any reliable data about it
        if (x >= min_x && x <= max_x &&
            y >= min_y && y <= max_y) {
            return true;
        }
        const float center_x = min_x + (max_x - min_x / 2.f);
        const float center_y = min_y + (max_y - min_y / 2.f);
        const float x2 = (x - center_x) * (x - center_x);
        const float y2 = (y - center_y) * (y - center_y);
        if (x2 + y2 <= range * range) {
            return true;
        }
        return false;
    }
};
}

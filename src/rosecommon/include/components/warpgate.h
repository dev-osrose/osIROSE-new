#pragma once

struct Warpgate {
    Warpgate(float x, float y, float z) : x_scale_(x), y_scale_(y), z_scale_(z) {}

    float x_scale_;
    float y_scale_;
    float z_scale_;
};

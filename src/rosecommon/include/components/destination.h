#pragma once

struct Destination {
    Destination(float x, float y, uint16_t dist, uint16_t target = 0) : x_(x), y_(y), dist_(dist), target_(target) {}

    float x_;
    float y_;
    uint16_t dist_;
    uint16_t target_;
};


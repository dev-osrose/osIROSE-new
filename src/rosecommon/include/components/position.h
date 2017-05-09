#pragma once

struct Position {
    Position() : x_(0), y_(0), z_(0), map_(0), spawn_(0) {}
    Position(float x, float y, uint16_t map, uint16_t spawn) : x_(x), y_(y), z_(0), map_(map), spawn_(spawn) {}

    template <typename T>
    Position(const T& row) : Position() {
        loadFromRow(row);
    }

    template <typename T>
    void loadFromRow(const T& row) {
        x_ = row.x;
        y_ = row.y;
        map_ = row.map;
        spawn_ = row.reviveMap;
    }

    float x_;
    float y_;
    uint16_t z_;
    uint16_t map_;
    uint16_t spawn_;
};


#pragma once

#include <array>

// This is not a component!
struct Buff {
    Buff() : id_(0), level_(0), value_(0) {}
    Buff(uint16_t id, uint8_t level, uint32_t value) : id_(id), level_(level), value_(value) {}


    // template <typename T>
    // void loadFromRow(const T& row) {
    //     id_ = row.id;
    //     level_ = row.level;
    // }

    uint16_t id_;
    uint8_t level_;
    uint32_t value_;
};

struct Buffs {
    static const uint16_t maxBuffs = 120;

    // template <typename T>
    // void loadFromResult(T& result) {
    //     size_t i = 0;
    //     for (const auto &row : result) {
    //         if (i >= maxBuffs)
    //             break;
    //         ++i;
    //         buffs_[i].loadFromRow(row);
    //     }
    // }

    std::array<Buff, maxBuffs> buffs_;
};


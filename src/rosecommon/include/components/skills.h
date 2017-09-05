#pragma once

#include <array>

// This is not a component!
struct Skill {
    Skill() : id_(0), level_(0) {}
    Skill(uint16_t id, uint8_t level) : id_(id), level_(level) {}


    template <typename T>
    void loadFromRow(const T& row) {
        id_ = row.id;
        level_ = row.level;
    }

    uint16_t id_;
    uint8_t level_;
};

struct Skills {
    static const uint16_t maxSkills = 120;

    template <typename T>
    void loadFromResult(T& result) {
        size_t i = 0;
        for (const auto &row : result) {
            if (i >= maxSkills)
                break;
            ++i;
            skills_[i].loadFromRow(row);
        }
    }

    std::array<Skill, maxSkills> skills_;
};


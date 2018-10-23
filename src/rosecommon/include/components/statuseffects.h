#pragma once

#include <vector>
#include <initializer_list>
#include <chrono>

// This is not a component!
struct StatusEffect {
    StatusEffect() : expired_(0), value_(0), unkown_(0), dt_(0) {}
    StatusEffect(std::chrono::seconds expired, uint16_t value, uint16_t unkown = 0) :
        expired_(expired), value_(value), unkown_(unkown), dt_(0) {}
    virtual ~StatusEffect() {}

    std::chrono::seconds expired_;
    uint16_t value_;
    uint16_t unkown_;
    std::chrono::milliseconds dt_; // to keep track of time
};

struct StatusEffects {
    static const uint16_t maxEffects = 40;

    StatusEffects() : effects_(maxEffects) {}
    StatusEffects(const std::initializer_list<StatusEffect> effects) : effects_(effects) {
        while (effects_.size() > maxEffects)
            effects_.pop_back();
        effects_.resize(maxEffects);
    }

    std::vector<StatusEffect> effects_;
};


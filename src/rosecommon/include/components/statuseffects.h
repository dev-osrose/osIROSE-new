#pragma once

#include <array>
#include <initializer_list>
#include <chrono>

#include "iserialize.h"
#include "crosewriter.h"

// This is not a component!
struct StatusEffect : public RoseCommon::ISerialize {
    StatusEffect() : expired_(0), value_(0), unkown_(0), dt_(0) {}
    StatusEffect(std::chrono::seconds expired, uint16_t value, uint16_t unkown = 0) :
        expired_(expired), value_(value), unkown_(unkown), dt_(0) {}
    virtual ~StatusEffect() {}

    std::chrono::seconds expired_;
    uint16_t value_;
    uint16_t unkown_;
    std::chrono::milliseconds dt_; // to keep track of time

    virtual bool write(RoseCommon::CRoseWriter& writer) const override {
        if (!writer.set_uint32_t(expired_.count())) return false;
        if (!writer.set_uint16_t(value_)) return false;
        return writer.set_uint16_t(unkown_);
    }

    virtual bool read(RoseCommon::CRoseReader& reader) override {
        uint32_t time;
        if (!reader.get_uint32_t(time)) return false;
        if (!reader.get_uint16_t(value_)) return false;
        return reader.get_uint16_t(unkown_);
    }

    virtual uint16_t get_size() const override {
        uint16_t size = 0;
        size += sizeof(uint32_t);
        size += sizeof(value_);
        size += sizeof(unkown_);
        return size;
    }
};

struct StatusEffects {
    static constexpr uint16_t maxEffects = 40;

    StatusEffects() {}
    template <typename... Args>
    StatusEffects(Args&&... effects) : effects_{effects...} {}

    std::array<StatusEffect, maxEffects> effects_;
};


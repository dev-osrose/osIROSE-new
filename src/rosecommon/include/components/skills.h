#pragma once

#include <array>
#include <initializer_list>
#include "crosepacket.h"

// This is not a component!
struct Skill : public RoseCommon::ISerialize {
    Skill() : id_(0), level_(0) {}
    Skill(uint16_t id, uint8_t level) : id_(id), level_(level) {}
    virtual ~Skill() {}

    uint16_t id_;
    uint8_t level_;

    protected:
        virtual void serialize(RoseCommon::CRosePacket &os) const {
            os << id_;
        }
        virtual void deserialize(RoseCommon::CRosePacket&) {}

};

struct Skills {
    static const uint16_t maxSkills = 120;

    Skills() {}
    Skills(const std::initializer_list<Skill> &skills) {
        int64_t diff = maxSkills - skills.size();
        diff = diff < 0 ? 0 : diff;
        auto last = skills.end() - diff;
        std::copy(skills.begin(), last, skills_.begin());
    }

    std::array<Skill, maxSkills> skills_;
};


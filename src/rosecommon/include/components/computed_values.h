#pragma once

#include "dataconsts.h"
#include <atomic>

namespace Component {
struct ComputedValues {
    ComputedValues() = default;
    ComputedValues(ComputedValues&&) = default;
    ~ComputedValues() = default;
    ComputedValues(const ComputedValues& other) : command(other.command), isOnMap(other.isOnMap.load()), moveMode(other.moveMode),
                                                  runSpeed(other.runSpeed), atkSpeed(other.atkSpeed), weightRate(other.weightRate) {}

    ComputedValues& operator=(ComputedValues&&) = default;
    ComputedValues& operator=(const ComputedValues& other) {
        command = other.command;
        isOnMap.store(other.isOnMap.load());
        moveMode = other.moveMode;
        runSpeed = other.runSpeed;
        atkSpeed = other.atkSpeed;
        weightRate = other.weightRate;
        return *this;
    }

    RoseCommon::Command command;
    mutable std::atomic_bool isOnMap;
    RoseCommon::MoveMode moveMode;
    uint16_t runSpeed;
    uint16_t atkSpeed;
    uint8_t weightRate;
};
}

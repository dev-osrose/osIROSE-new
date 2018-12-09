#pragma once

#include "dataconsts.h"
#include <atomic>

namespace Component {
struct ComputedValues {
    RoseCommon::Command command;
    mutable std::atomic_bool isOnMap;
    RoseCommon::moveMode moveMode;
    uint16_t runSpeed;
    uint16_t atkSpeed;
    uint8_t weightRate;
};
}

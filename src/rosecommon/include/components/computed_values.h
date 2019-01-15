#pragma once

#include "dataconsts.h"

namespace Component {
struct ComputedValues {
    RoseCommon::Command command;
    RoseCommon::MoveMode moveMode;
    uint16_t runSpeed;
    uint16_t atkSpeed;
    uint8_t weightRate;
    uint32_t statusFlag;
    uint32_t subFlag;
};
}

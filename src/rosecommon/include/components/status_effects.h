#pragma once

#include "dataconsts.h"
#include <array>

namespace Component {
struct StatusEffects {
    std::array<RoseCommon::StatusEffect, RoseCommon::MAX_STATUS_EFFECTS> effects;
};
}

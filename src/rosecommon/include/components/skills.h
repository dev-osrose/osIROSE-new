#pragma once

#include "dataconsts.h"
#include <array>

namespace Component {
struct Skills {
    std::array<RoseCommon::Skill, RoseCommon::MAX_SKILL_COUNT> skills;
};
}

#pragma once

#include <memory>
#include "party_base.h"

namespace Component {
struct Party {
    std::shared_ptr<RoseCommon::PartyBase> party;
    bool isKicked;
    bool isRequested;
};
}

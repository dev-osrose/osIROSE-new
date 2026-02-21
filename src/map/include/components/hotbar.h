#pragma once

#include <array>
#include <srv_select_char_reply.h>
#include "dataconsts.h"

namespace Component {
  struct Hotbar {
    std::array<RoseCommon::Packet::SrvSelectCharReply::HotbarItem, RoseCommon::MAX_HOTBAR_ITEMS> items;
  };
}

//ALL ADDED BY DAVIDIXX

#pragma once

#include "cli_stat_add_req.h"
#include "dataconsts.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;

class EntitySystem;

namespace Player {
void add_stat(EntitySystem&, RoseCommon::Entity, const RoseCommon::Packet::CliStatAddReq&);
}

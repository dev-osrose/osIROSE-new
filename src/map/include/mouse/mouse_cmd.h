#pragma once

#include "dataconsts.h"
#include "cli_mouse_cmd.h"

class EntitySystem;

namespace Mouse {
void mouse_cmd(EntitySystem&, RoseCommon::Entity, const RoseCommon::Packet::CliMouseCmd&);
}

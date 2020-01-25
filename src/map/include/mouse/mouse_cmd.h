#pragma once

#include "dataconsts.h"
#include "cli_mouse_cmd.h"
#include "cli_stop_moving.h"

class EntitySystem;

namespace Mouse {
void mouse_cmd(EntitySystem&, RoseCommon::Entity, const RoseCommon::Packet::CliMouseCmd&);
void stop_moving(EntitySystem&, RoseCommon::Entity, const RoseCommon::Packet::CliStopMoving&);
}

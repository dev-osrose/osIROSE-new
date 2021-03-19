#pragma once

#include "entity.h"
#include "cli_mouse_cmd.h"
#include "cli_stop_moving.h"

class EntitySystem;

namespace Mouse {
void mouse_cmd(EntitySystem&, Entity, const RoseCommon::Packet::CliMouseCmd&);
void stop_moving(EntitySystem&, Entity, const RoseCommon::Packet::CliStopMoving&);
}

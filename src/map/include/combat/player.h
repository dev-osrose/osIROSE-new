#pragma once

#include "cli_stat_add_req.h"
#include "cli_set_animation.h"
#include "cli_toggle_move.h"
#include "entity.h"
#include <optional>


class EntitySystem;

namespace Player {
void add_stat(EntitySystem&, Entity, const RoseCommon::Packet::CliStatAddReq&);
void toggle_player_move(EntitySystem&, Entity, const RoseCommon::Packet::CliToggleMove&);
void set_animation(EntitySystem&, Entity, const RoseCommon::Packet::CliSetAnimation&);
}

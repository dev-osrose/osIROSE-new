#pragma once

#include "entity.h"
#include "cli_normal_chat.h"

class EntitySystem;

namespace Chat {
void normal_chat(EntitySystem&, Entity, const RoseCommon::Packet::CliNormalChat&);
}

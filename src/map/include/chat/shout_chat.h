#pragma once

#include "entity.h"
#include "cli_shout_chat.h"
#include "srv_announce_chat.h"

class EntitySystem;

namespace Chat {
void shout_chat(EntitySystem&, Entity, const RoseCommon::Packet::CliShoutChat&);
void announce_chat_srv(EntitySystem&, Entity, const RoseCommon::Packet::SrvAnnounceChat&);
}

#pragma once

#include "dataconsts.h"
#include "cli_shout_chat.h"
#include "srv_shout_chat.h"

class EntitySystem;

namespace Chat {
void shout_chat(EntitySystem&, RoseCommon::Entity, const RoseCommon::Packet::CliShoutChat&);
void shout_chat_srv(EntitySystem&, RoseCommon::Entity, const RoseCommon::Packet::SrvShoutChat&);
}

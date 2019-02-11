#pragma once

#include "dataconsts.h"
#include "cli_party_chat.h"

class EntitySystem;

namespace Chat {
void party_chat(EntitySystem&, RoseCommon::Entity, const RoseCommon::Packet::CliPartyChat&);
}

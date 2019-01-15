#pragma once

#include "dataconsts.h"
#include "cli_whisper_chat.h"

class EntitySystem;

namespace Chat {
void whisper_chat(EntitySystem&, RoseCommon::Entity, const RoseCommon::Packet::CliWhisperChat&);

void send_whisper(EntitySystem& entitySystem, Entity entity, const std::string& msg);
}

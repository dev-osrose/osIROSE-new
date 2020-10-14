#pragma once

#include "entity.h"
#include "cli_whisper_chat.h"
#include "srv_whisper_chat.h"

class EntitySystem;

namespace Chat {
void whisper_chat(EntitySystem&, Entity, const RoseCommon::Packet::CliWhisperChat&);
void whisper_chat_srv(EntitySystem&, Entity, const RoseCommon::Packet::SrvWhisperChat&);

void send_whisper(EntitySystem& entitySystem, Entity entity, const std::string& msg);
}

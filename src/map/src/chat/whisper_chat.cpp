#include "chat/whisper_chat.h"

#include "logconsole.h"
#include "entity_system.h"
#include "cli_whisper_chat.h"
#include "srv_whisper_chat.h"
#include "components/basic_info.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;

void Chat::whisper_chat(EntitySystem& entitySystem, Entity entity, const CliWhisperChat& packet) {
    auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
    logger->trace("Chat::whisper_chat()");
    logger->trace("entity {}, whisper to {} message {}", entity, packet.get_target(), packet.get_message());
    const auto other = entitySystem.get_entity_from_name(packet.get_target());
    if (other == entt::null) {
        logger->debug("target {} is not online", packet.get_target());
        return;
    }
    const auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(entity);
    auto p = SrvWhisperChat::create(basicInfo.name, packet.get_message());
    entitySystem.send_to(other, p);
}

void Chat::send_whisper(EntitySystem& entitySystem, Entity entity, const std::string& msg) {
    auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
    logger->trace("Chat::send_whisper()");
    logger->trace("sending '{}' to {}", msg, entity);
    auto p = SrvWhisperChat::create("server msg", msg);
    entitySystem.send_to(entity, p);
}

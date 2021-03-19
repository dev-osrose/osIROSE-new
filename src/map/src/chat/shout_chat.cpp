#include "chat/shout_chat.h"

#include "logconsole.h"
#include "entity_system.h"
#include "components/basic_info.h"
#include "cli_shout_chat.h"
#include "srv_shout_chat.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;

void Chat::shout_chat(EntitySystem& entity_system, Entity entity, const CliShoutChat& packet) {
    auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
    logger->trace("Chat::shout_chat()");
    logger->debug("entity {}, message {}", entity, packet.get_message());

    const auto& info = entity_system.get_component<Component::BasicInfo>(entity);

    auto p = SrvShoutChat::create(info.name, packet.get_message());
    entity_system.send_map(p);
}

void Chat::announce_chat_srv(EntitySystem& entity_system, Entity, const RoseCommon::Packet::SrvAnnounceChat& packet) {
    auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
    logger->trace("Chat::announce_chat_srv()");
    logger->debug("name {}, message {}", packet.get_name(), packet.get_message());
    entity_system.send_map(packet);
}

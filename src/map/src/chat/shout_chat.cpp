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
    logger->trace("entity {}, message {}", entity, packet.get_message());

    const auto& info = entity_system.get_component<Component::BasicInfo>(entity);

    auto p = SrvShoutChat::create(info.name, packet.get_message());
    entity_system.send_map(p);
}

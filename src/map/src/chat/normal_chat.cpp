#include "chat/normal_chat.h"

#include "logconsole.h"
#include "entity_system.h"
#include "cli_normal_chat.h"
#include "srv_normal_chat.h"
#include "components/basic_info.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;

void Chat::normal_chat(EntitySystem& entitySystem, Entity entity, const CliNormalChat& packet) {
    auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
    logger->trace("Chat::normal_chat()");
    logger->trace("entity {}, message {}", entity, packet.get_message());
    const auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(entity);
    auto p = SrvNormalChat::create(basicInfo.id, packet.get_message());
    entitySystem.send_nearby(entity, p);
}

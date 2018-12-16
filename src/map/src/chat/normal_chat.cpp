#include "chat/normal_chat.h"

#include "logconsole.h"
#include "entity_system.h"
#include "cli_normal_chat.h"
#include "srv_normal_chat.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;

void Chat::normal_chat(EntitySystem& entitySystem, Entity entity, const CliNormalChat& packet) {
    auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
    logger->trace("Chat::normal_chat()");
    logger->debug("entity {}, message {}", entity, packet.get_message());
}

#include "chat/party_chat.h"

#include "logconsole.h"
#include "entity_system.h"
#include "srv_party_chat.h"
#include "components/basic_info.h"
#include "components/party.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;

void Chat::party_chat(EntitySystem& entitySystem, Entity entity, const CliPartyChat& packet) {
    auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
    logger->trace("Chat::party_chat()");
    logger->trace("entity {}, message {}", entity, packet.get_message());
    const auto* p = entitySystem.try_get_component<Component::Party>(entity);
    if (!p || !p->party) {
        logger->warn("Client {} tried to send a party chat but there is no party", entity);
        return;
    }
    const auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(entity);
    const auto pp = SrvPartyChat::create(basicInfo.id, packet.get_message());
    for (const auto& it : p->members) {
        entitySystem.send_to(it, pp);
    }
}

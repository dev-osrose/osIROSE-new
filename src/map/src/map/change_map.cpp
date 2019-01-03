#include "change_map.h"

#include "logconsole.h"
#include "entity_system.h"
#include "cli_normal_chat.h"
#include "srv_normal_chat.h"
#include "cli_change_map_req.h"
#include "components/basic_info.h"
#include "whisper_char.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;

void Map::change_map_request(EntitySystem& entitySystem, Entity entity, const CliChangeMapReq&) {
    auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
    logger->trace("Map::change_map_request");
    logger->trace("entity {}", entity);
    entitySystem.send_to(entity, SrvChangeMapReply::create());
    const auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(entity);
    Chat::send_whisper(entitySystem, entity, fmt::format("You are client {}", basicInfo.id));
    entitySystem.send_nearby_except_me(SrvPlayerChar::create());
    const auto& nearby_entities = entitySystem.get_nearby(entity);
    for (auto e : nearby_entities) {
        if (e != entity) {
        entitySystem.send_to(entity, SrvPlayerChar::create());
        }
    }
    if (const auto client_ptr = registry.try_get<const Component::Client>(entity)) {
        if (auto client = client_ptr->client.lock()) {
            client->set_logged_in();
        }
    }
}

#include "map/change_map.h"

#include "logconsole.h"
#include "entity_system.h"
#include "cli_change_map_req.h"
#include "srv_change_map_reply.h"
#include "components/basic_info.h"
#include "chat/whisper_chat.h"
#include "cmapclient.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;

void Map::change_map_request(EntitySystem& entitySystem, Entity entity, const CliChangeMapReq&) {
    auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
    logger->trace("Map::change_map_request");
    logger->trace("entity {}", entity);
    const auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(entity);
    const auto& life = entitySystem.get_component<Component::Life>(entity);
    const auto& mana = entitySystem.get_component<Component::Mana>(entity);
    const auto& level = entitySystem.get_component<Component::Level>(entity);
    entitySystem.send_to(entity, SrvChangeMapReply::create(
        basicInfo.id, life.hp, mana.mp, level.xp, level.penalize_xp,
        entitySystem.get_world_time(), basicInfo.teamId)); // FIXME: /!\ teamId is uint32_t but the packet expects a uint16_t /!\
    Chat::send_whisper(entitySystem, entity, fmt::format("You are client {}", basicInfo.id));
    entitySystem.send_nearby_except_me(CMapClient::create_srv_player_char(entitySystem, entity));
    const auto& nearby_entities = entitySystem.get_nearby(entity);
    for (auto other : nearby_entities) {
        if (other != entity) {
        entitySystem.send_to(entity, CMapClient::create_srv_player_char(entitySystem, entity));
        }
    }
    if (const auto client_ptr = registry.try_get<const Component::Client>(entity)) {
        if (auto client = client_ptr->client.lock()) {
            client->set_logged_in();
        }
    }
}

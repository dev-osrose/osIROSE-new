#include "map/change_map.h"

#include "logconsole.h"
#include "entity_system.h"
#include "cli_change_map_req.h"
#include "srv_change_map_reply.h"
#include "chat/whisper_chat.h"
#include "cmapclient.h"

#include "components/basic_info.h"
#include "components/client.h"
#include "components/destination.h"
#include "components/level.h"
#include "components/life.h"
#include "components/magic.h"
#include "components/position.h"
#include "components/warpgate.h"
#include "isc_client_status.h"

#include <limits>

using namespace RoseCommon;
using namespace RoseCommon::Packet;

void Map::change_map_request(EntitySystem& entitySystem, Entity entity, const CliChangeMapReq&) {
    auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
    logger->trace("Map::change_map_request");
    logger->trace("entity {}", entity);
    const auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(entity);
    const auto& life = entitySystem.get_component<Component::Life>(entity);
    const auto& magic = entitySystem.get_component<Component::Magic>(entity);
    const auto& level = entitySystem.get_component<Component::Level>(entity);
    // force send the packet as the client isn't technically on the map yet
    entitySystem.send_to(entity, SrvChangeMapReply::create(
        basicInfo.id, life.hp, magic.mp, level.xp, level.penaltyXp,
        entitySystem.get_world_time(), basicInfo.teamId), true); // FIXME: teamId is uint32_t but the packet expects a uint16_t
    if (const auto client_ptr = entitySystem.try_get_component<Component::Client>(entity)) {
        if (auto client = client_ptr->client.lock()) {
            client->set_on_map();
        }
    }
    Chat::send_whisper(entitySystem, entity, fmt::format("You are client {}", basicInfo.id));
    entitySystem.send_nearby_except_me(entity, CMapClient::create_srv_player_char(entitySystem, entity));
    const auto& nearby_entities = entitySystem.get_nearby(entity);
    for (auto other : nearby_entities) {
        if (const auto client_ptr = entitySystem.try_get_component<Component::Client>(other)) {
            if (auto client = client_ptr->client.lock()) {
                if (!client->is_on_map()) {
                    continue;
                }
            }
        }
        entitySystem.send_to_entity(entity, other);
    }
    entitySystem.send_to_char_server(IscClientStatus::create(basicInfo.id, IscClientStatus::CONNECTED), basicInfo.charId);
}

void Map::teleport_request(EntitySystem& entitySystem, Entity entity, const RoseCommon::Packet::CliTeleportReq& packet) {
    auto logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
    logger->trace("Map::teleport_request");
    logger->trace("entity {}", entity);
    const auto& pos = entitySystem.get_component<Component::Position>(entity);
    logger->trace("packet position ({},{}), server position ({},{})", packet.get_x(), packet.get_y(), pos.x, pos.y);
    float closest = std::numeric_limits<float>::max();
    Component::Warpgate warp;
    Component::Destination dest;
    for (const auto w : entitySystem.get_entities_with_components<Component::Warpgate>()) {
        const auto [cx, cy, _] = entitySystem.get_component<Component::Warpgate>(w).get_center();
        float dist = (cx - pos.x) * (cx - pos.x) + (cy - pos.y) * (cy - pos.y);
        if (dist < closest) {
            closest = dist;
            warp = entitySystem.get_component<Component::Warpgate>(w);
            dest = entitySystem.get_component<Component::Destination>(w);
        }
        /*// TODO: check for hack, do something with the warpgate center and the current position?
        const auto& warpgate = entitySystem.get_component<Component::Warpgate>(w);
        if (warpgate.id == packet.get_id()) {
            const auto& dest = entitySystem.get_component<Component::Destination>(w);
            entitySystem.teleport_entity(entity, dest.x, dest.y, warpgate.dest_map);
            break;
        }*/
    }
    logger->trace("closest warpgate ({},{}), ({},{})", warp.min_x, warp.min_y, warp.max_x, warp.max_y);
    entitySystem.teleport_entity(entity, dest.x, dest.y, warp.dest_map);
}

#include "systems/mapsystem.h"
#include "systems/chatsystem.h"
#include "cmapclient.h"
#include "cmapserver.h"

using namespace Systems;
using namespace RoseCommon;

MapSystem::MapSystem(SystemManager &m) : System(m) {
    m.registerDispatcher(ePacketType::PAKCS_CHANGE_MAP_REQ, &MapSystem::processChangeMapReq);
}

void MapSystem::update(EntityManager&, double) {}

void MapSystem::processChangeMapReq(CMapClient *client, Entity entity, const RoseCommon::CliChangeMapReq&) {
    logger_->trace("MapSystem::processChangeMapRequest");

    auto advanced = entity.component<AdvancedInfo>();
    auto basic = entity.component<BasicInfo>();
    auto info = entity.component<CharacterInfo>();

    client->Send(
            *makePacket<ePacketType::PAKWC_CHANGE_MAP_REPLY>(
                getId(entity), advanced->hp_, advanced->mp_, basic->xp_, info->penaltyXp_, std::time(nullptr), 0));
    manager_.get<Systems::ChatSystem>()->sendMsg(entity, "You are client n " + std::to_string(getId(entity)));
    CMapServer::SendPacket(client, CMapServer::eSendType::EVERYONE_BUT_ME,
            *makePacket<ePacketType::PAKWC_PLAYER_CHAR>(entity));
    auto &manager = manager_.getEntityManager();
    for (Entity e : manager.entities_with_components(basic)) {
        if (e != entity && basic->loggedIn_.load())
            client->Send(*makePacket<ePacketType::PAKWC_PLAYER_CHAR>(entity));
    }
    entity.component<BasicInfo>()->loggedIn_.store(true);
}

#include "systems/mapsystem.h"
#include "systems/chatsystem.h"

using namespace Systems;

MapSystem::MapSystem(SystemManager &m) : System(m) {}

void MapSystem::update(EntityManager&, double) {}

void MapSystem::processChangeMapRequest(CMapClient *client, Entity entity, const RoseCommon::CliChangeMapReq &packet) {
    logger_->trace("MapSystem::processChangeMapRequest");

    auto advanced = entity_.component<AdvancedInfo>();
    auto basic = entity_.component<BasicInfo>();
    auto info = entity_.component<CharacterInfo>();

    client->Send(
            *makePacket<ePacketType::PAKWC_CHANGE_MAP_REPLY>(
                GetId(), advanced->hp_, advanced->mp_, basic->xp_, info->penaltyXp_, std::time(nullptr), 0));
    systemManager_.get<Systems::ChatSystem>()->sendMsg(entity_, "You are client n° " + std::to_string(GetId()));
    CMapServer::SendPacket(this, CMapServer::eSendType::EVERYONE_BUT_ME,
            *makePacket<ePacketType::PAKWC_PLAYER_CHAR>(entity));
    auto manager = systemManager_.getEntityManager();
    for (Entity e : manager.entities_with_components(basic)) {
        if (e != entity && basic->loggedIn_.load())
            client->Send(*makePacket<ePacketType::PAKWC_PLAYER_CHAR>(entity));
    }
}

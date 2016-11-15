#include "systems/chatsystem.h"
#include "cmapserver.h"

using namespace Systems;
using namespace RoseCommon;

ChatSystem::ChatSystem(SystemManager &manager) {
    manager.registerDispatcher(ePacketType::PAKCS_NORMAL_CHAT, &ChatSystem::normalChat);
}

void ChatSystem::update(EntityManager&, double) {}

void ChatSystem::normalChat(CMapClient *client, Entity entity, const RoseCommon::CliNormalChat &packet) {
    logger_->trace("ChatSystem::normalChat");
    if (!client || !entity.component<BasicInfo>())
        return;
    uint16_t id = entity.component<BasicInfo>()->id_;
    CMapServer::SendPacket(client, CMapServer::eSendType::NEARBY_BUT_ME, 
            *makePacket<ePacketType::PAKWC_NORMAL_CHAT>(id, packet.message()));
}

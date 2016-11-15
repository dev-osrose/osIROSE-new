#include "systems/chatsystem.h"
#include "cmapserver.h"
#include "cmapclient.h"

using namespace Systems;
using namespace RoseCommon;

ChatSystem::ChatSystem(SystemManager &manager) {
    manager.registerDispatcher(ePacketType::PAKCS_NORMAL_CHAT, &ChatSystem::normalChat);
    manager.registerDispatcher(ePacketType::PAKCS_WHISPER_CHAT, &ChatSystem::whisperChat);
}

void ChatSystem::update(EntityManager&, double) {}

void ChatSystem::normalChat(EntityManager&, CMapClient *client, Entity entity, const CliNormalChat &packet) {
    logger_->trace("ChatSystem::normalChat");
    if (!client || !entity.component<BasicInfo>())
        return;
    uint16_t id = entity.component<BasicInfo>()->id_;
    CMapServer::SendPacket(client, CMapServer::eSendType::NEARBY, 
            *makePacket<ePacketType::PAKWC_NORMAL_CHAT>(id, packet.message()));
}

void ChatSystem::whisperChat(EntityManager &es, CMapClient *client, Entity entity, const CliWhisperChat &packet) {
    logger_->trace("ChatSystem::whisperChat");
    if (!client || !entity.component<BasicInfo>())
        return;
    Component<BasicInfo> basicInfo;
    Component<SocketConnector> socket;
    for (Entity target : es.entities_with_components(basicInfo, socket)) {
        (void)target;
        if (basicInfo->name_ == packet.targetId()) {
            socket->client_->Send(*makePacket<ePacketType::PAKWC_WHISPER_CHAT>(packet.message(), entity.component<BasicInfo>()->name_));
            return;
        }
    }
    client->Send(*makePacket<ePacketType::PAKWC_WHISPER_CHAT>("", "User cannot be found or is offline"));
}

#include "systems/chatsystem.h"
#include "cmapserver.h"
#include "cmapclient.h"

using namespace Systems;
using namespace RoseCommon;

ChatSystem::ChatSystem(SystemManager &manager) : System(manager) {
    manager.registerDispatcher(ePacketType::PAKCS_NORMAL_CHAT, &ChatSystem::normalChat);
    manager.registerDispatcher(ePacketType::PAKCS_WHISPER_CHAT, &ChatSystem::whisperChat);
}

void ChatSystem::update(EntityManager&, double) {}

void ChatSystem::normalChat(CMapClient *client, Entity entity, const CliNormalChat &packet) {
    logger_->trace("ChatSystem::normalChat");
    if (!client || !entity.component<BasicInfo>())
        return;
    uint16_t id = entity.component<BasicInfo>()->id_;
    CMapServer::SendPacket(client, CMapServer::eSendType::NEARBY, 
            *makePacket<ePacketType::PAKWC_NORMAL_CHAT>(id, packet.message()));
}

void ChatSystem::whisperChat(CMapClient *client, Entity entity, const CliWhisperChat &packet) {
    logger_->trace("ChatSystem::whisperChat");
    if (!client || !entity.component<BasicInfo>())
        return;
    auto target = manager_.getEntity(packet.targetId());
    if (!target || !target.component<SocketConnector>() || !target.component<SocketConnector>()->client_) {
        client->Send(*makePacket<ePacketType::PAKWC_WHISPER_CHAT>("", "User cannot be found or is offline"));
        return;
    }
    target.component<SocketConnector>()->client_->Send(
            *makePacket<ePacketType::PAKWC_WHISPER_CHAT>(packet.message(), entity.component<BasicInfo>()->name_));
}

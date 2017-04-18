#include "systems/chatsystem.h"
#include "cmapserver.h"
#include "cmapclient.h"

using namespace Systems;
using namespace RoseCommon;

ChatSystem::ChatSystem(SystemManager &manager) : System(manager) {
    manager.registerDispatcher(ePacketType::PAKCS_NORMAL_CHAT, &ChatSystem::normalChat);
    manager.registerDispatcher(ePacketType::PAKCS_WHISPER_CHAT, &ChatSystem::whisperChat);
    manager.registerDispatcher(ePacketType::PAKCS_PARTY_CHAT, &ChatSystem::partyChat);
}

void ChatSystem::update(EntityManager&, double) {}

void ChatSystem::normalChat(CMapClient *client, Entity entity, const CliNormalChat &packet) {
    logger_->trace("ChatSystem::normalChat");
    if (!client || !entity.component<BasicInfo>())
        return;
    CMapServer::SendPacket(client, CMapServer::eSendType::NEARBY, 
            *makePacket<ePacketType::PAKWC_NORMAL_CHAT>(getId(entity), packet.message()));
}

void ChatSystem::whisperChat(CMapClient *client, Entity entity, const CliWhisperChat &packet) {
    logger_->trace("ChatSystem::whisperChat");
    if (!client || !entity.component<BasicInfo>())
        return;
    auto target = manager_.getEntity(packet.targetId());
    if (!target || !getClient(target)) {
        client->send(*makePacket<ePacketType::PAKWC_WHISPER_CHAT>("", "User cannot be found or is offline"));
        return;
    }
    if (auto socket = getClient(target))
        socket->send(
                    *makePacket<ePacketType::PAKWC_WHISPER_CHAT>(getName(entity), packet.message()));
}

void ChatSystem::partyChat(CMapClient *client, Entity entity, const CliPartyChat &packet) {
    logger_->trace("ChatSystem::partyChat");
    if (!client || !entity.component<BasicInfo>())
        return;
    if (!entity.component<Party>() || !entity.component<Party>()->party_) {
        logger_->warn("Client {} tried to send a party message but isn't in a party", getId(entity));
        return;
    }
    auto party = entity.component<Party>()->party_;
    for (auto it : party->members_) {
        if (auto socket = getClient(it))
            socket->send(*makePacket<ePacketType::PAKWC_PARTY_CHAT>(getId(entity), packet.message()));
    }
}

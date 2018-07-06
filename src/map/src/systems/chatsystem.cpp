#include <sstream>

#include "systems/chatsystem.h"
#include "cli_normalchat.h"
#include "cli_partychat.h"
#include "cli_whisperchat.h"
#include "cmapclient.h"
#include "cmapserver.h"
#include "itemdb.h"
#include "srv_normalchat.h"
#include "srv_partychat.h"
#include "srv_whisperchat.h"
#include "systems/inventorysystem.h"

using namespace Systems;
using namespace RoseCommon;

ChatSystem::ChatSystem(SystemManager &manager) : System(manager) {
  manager.registerDispatcher(ePacketType::PAKCS_NORMAL_CHAT, &ChatSystem::normalChat);
  manager.registerDispatcher(ePacketType::PAKCS_WHISPER_CHAT, &ChatSystem::whisperChat);
  manager.registerDispatcher(ePacketType::PAKCS_PARTY_CHAT, &ChatSystem::partyChat);
}

void ChatSystem::update(EntityManager &, double) {}

void ChatSystem::sendMsg(Entity entity, const std::string &msg) {
  logger_->trace("ChatSystem::sendMsg");
  if (!entity) return;
  if (auto client = getClient(entity)) client->send(*makePacket<ePacketType::PAKWC_WHISPER_CHAT>("server", msg));
}

void ChatSystem::normalChat(CMapClient &client, Entity entity, const CliNormalChat &packet) {
  logger_->trace("ChatSystem::normalChat");
  if (!entity.component<BasicInfo>()) return;
  if (packet.message()[0] == '/') {
    logger_->info("GM command from {} : {}", getId(entity), packet.message());
    std::stringstream ss(packet.message());
    std::string command;
    ss >> command;
    if (command == "/item") {
      uint16_t type = 0, id = 0;
      ss >> type >> id;
      if (!type || !id) {
        logger_->info("Wrong number of arguments for GM command {} from {}", packet.message(), getId(entity));
        client.send(*makePacket<ePacketType::PAKWC_WHISPER_CHAT>("", "Usage: /item <type> <id>"));
        return;
      } else if (!ItemDatabase::getInstance().itemExists(type, id)) {
        logger_->info("Wrong type, id: {}, {}", type, id);
        client.send(*makePacket<ePacketType::PAKWC_WHISPER_CHAT>("", fmt::format("{} {} doesn't exists", type, id)));
        return;
      }
      auto invSys = manager_.get<InventorySystem>();
      auto item = invSys->buildItem(type, id);
      if (!item) {
        client.send(
            *makePacket<ePacketType::PAKWC_WHISPER_CHAT>("", "Error while building the object, check the logs"));
      } else if (!InventorySystem::addItem(entity, std::move(item.value()))) {
        logger_->info("Inventory full for {}", getId(entity));
        client.send(*makePacket<ePacketType::PAKWC_WHISPER_CHAT>("", "Inventory full"));
      } else {
        logger_->info("Item {}:{} added to {}", type, id, getId(entity));
        client.send(*makePacket<ePacketType::PAKWC_WHISPER_CHAT>("", fmt::format("Item {}:{} added", type, id)));
      }
    } else if (command == "/load_npc") {
      client.get_entity_system()->get_script_loader().load_npcs(); //FIXME : temporary to show how we could do it
    }
    return;
  }
  manager_.SendPacket(client, CMapServer::eSendType::NEARBY,
                         *makePacket<ePacketType::PAKWC_NORMAL_CHAT>(getId(entity), packet.message()));
}

void ChatSystem::whisperChat(CMapClient &client, Entity entity, const CliWhisperChat &packet) {
  logger_->trace("ChatSystem::whisperChat");
  if (!entity.component<BasicInfo>()) return;
  auto target = manager_.getEntity(packet.targetId());
  if (!target || !getClient(target)) {
    client.send(*makePacket<ePacketType::PAKWC_WHISPER_CHAT>("", "User cannot be found or is offline"));
    return;
  }
  if (auto socket = getClient(target))
    socket->send(*makePacket<ePacketType::PAKWC_WHISPER_CHAT>(getName(entity), packet.message()));
}

void ChatSystem::partyChat(CMapClient &, Entity entity, const CliPartyChat &packet) {
  logger_->trace("ChatSystem::partyChat");
  if (!entity.component<BasicInfo>()) return;
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

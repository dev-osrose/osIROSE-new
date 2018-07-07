#include "gm_commands.h"

#include <sstream>
#include <unordered_map>
#include <functional>

#include "logconsole.h"
#include "cmapclient.h"
#include "cmapserver.h"
#include "systems/inventorysystem.h"
#include "systems/chatsystem.h"
#include "itemdb.h"
#include "srv_whisperchat.h"
#include "systems/inventorysystem.h"
#include "gm_commands.h"

using namespace Systems;
using namespace RoseCommon;

namespace {

void item(std::stringstream &&ss, SystemManager &manager, Entity e) {
    auto logger = Core::CLog::GetLogger(Core::log_type::SYSTEM).lock();
    auto client = getClient(e);
    uint16_t type = 0, id = 0;
    ss >> type >> id;
    if (!type || !id) {
        logger->info("Wrong number of arguments for GM command {} from {}", fmt::format("/item {}", ss.str()), getId(e));
        client->send(*makePacket<ePacketType::PAKWC_WHISPER_CHAT>("", "Usage: /item <type> <id>"));
        return;
    } else if (!ItemDatabase::getInstance().itemExists(type, id)) {
        logger->info("Wrong type, id: {}, {}", type, id);
        client->send(*makePacket<ePacketType::PAKWC_WHISPER_CHAT>("", fmt::format("{} {} doesn't exists", type, id)));
        return;
    }
    auto inv = manager.get<InventorySystem>();
    auto item = inv->buildItem(type, id);
    if (!item) {
        client->send(*makePacket<ePacketType::PAKWC_WHISPER_CHAT>("", "Error while building the object, check the logs"));
    } else if (!InventorySystem::addItem(e, std::move(item.value()))) {
        logger->info("Inventory full for {}", getId(e));
        client->send(*makePacket<ePacketType::PAKWC_WHISPER_CHAT>("", "Inventory full"));
    } else {
        logger->info("Item {}:{} added to {}", type, id, getId(e));
        client->send(*makePacket<ePacketType::PAKWC_WHISPER_CHAT>("", fmt::format("Item {}:{} added", type, id)));
    }
}

void load_npc(std::stringstream&&, SystemManager&, Entity e) {
    auto client = getClient(e);
    client->get_entity_system()->get_script_loader().load_npcs();
}

std::unordered_map<std::string, std::pair<std::function<void(std::stringstream &&ss, SystemManager&, Entity)>, std::string>> commands = {
    {"/item", {item, "Creates an item. Usage: /item <type> <id>"}},
    {"/load_npc", {load_npc, "Loads npc(s). TODO"}}
};

void help(Entity entity) {
    auto client = getClient(entity);
    std::string text;
    for (const auto &[key, value] : commands) {
        text += key + "\t" + value.second + "\n";
    }
    client->send(*makePacket<ePacketType::PAKWC_WHISPER_CHAT>("", text));
}

}

bool executeGM(const std::string& message, SystemManager &manager, Entity entity) {
    if (message[0] != '/') return false;
    if (message == "/help") {
        help(entity);
        return true;
    }
    std::stringstream ss(message);
    std::string command;
    ss >> command;
    if (auto it = commands.find(command); it != commands.end()) {
        it->second.first(std::move(ss), manager, entity);
        return true;
    }
    return false;
}

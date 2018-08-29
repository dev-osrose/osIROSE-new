#include "gm_commands.h"

#include <limits.h>
#include <sstream>
#include <unordered_map>
#include <functional>

#include "logconsole.h"
#include "cmapclient.h"
#include "cmapserver.h"
#include "itemdb.h"
#include "dataconsts.h"
#include "srv_whisperchat.h"
#include "systems/chatsystem.h"
#include "systems/inventorysystem.h"
#include "systems/movementsystem.h"
#include "systems/combat_system.h"
#include "gm_commands.h"

using namespace Systems;
using namespace RoseCommon;

namespace {
void help(Entity entity, const std::string& command = "");

void item(std::stringstream &&ss, SystemManager &manager, Entity e) {
    auto logger = Core::CLog::GetLogger(Core::log_type::SYSTEM).lock();
    auto client = getClient(e);
    uint16_t type = 0, id = 0;
    ss >> type >> id;
    if (!type || !id) {
        logger->info("Wrong number of arguments for GM command {} from {}", fmt::format("/item {}", ss.str()), getId(e));
        help(e, "/item");
        return;
    } else if (!ItemDatabase::getInstance().itemExists(type, id)) {
        logger->info("Wrong type, id: {}, {}", type, id);
        client->send(makePacket<ePacketType::PAKWC_WHISPER_CHAT>("", fmt::format("{} {} doesn't exists", type, id)));
        return;
    }
    auto inv = manager.get<InventorySystem>();
    auto item = inv->buildItem(type, id);
    if (!item) {
        client->send(makePacket<ePacketType::PAKWC_WHISPER_CHAT>("", "Error while building the object, check the logs"));
    } else if (!InventorySystem::addItem(e, std::move(item.value()))) {
        logger->info("Inventory full for {}", getId(e));
        client->send(makePacket<ePacketType::PAKWC_WHISPER_CHAT>("", "Inventory full"));
    } else {
        logger->info("Item {}:{} added to {}", type, id, getId(e));
        client->send(makePacket<ePacketType::PAKWC_WHISPER_CHAT>("", fmt::format("Item {}:{} added", type, id)));
    }
}

void load_npc(std::stringstream&&, SystemManager&, Entity) {
    //TODO: todo
}

void teleport(std::stringstream&& ss, SystemManager &manager, Entity e) {
    auto logger = Core::CLog::GetLogger(Core::log_type::SYSTEM).lock();
    auto client = getClient(e);
    uint16_t map_id = 0;
    float x = 0.f, y = 0.f;
    ss >> map_id >> x >> y;
    if (!map_id || !x || !y) {
        logger->info("Wrong number of arguments for GM command {} from {}", fmt::format("/tp {}", ss.str()), getId(e));
        help(e, "/tp");
        return;
    }
    manager.get<MovementSystem>()->teleport(e, map_id, x * 100, y * 100); // transforms from map coordinates to real coordinates
}

void die(std::stringstream&& ss, SystemManager &manager, Entity e) {
    auto logger = Core::CLog::GetLogger(Core::log_type::SYSTEM).lock();
    logger->trace("gm_commands die called");
    auto client = getClient(e);
    manager.get<CombatSystem>()->apply_damage(e, MAX_DAMAGE);
}

std::unordered_map<std::string, std::pair<std::function<void(std::stringstream &&ss, SystemManager&, Entity)>, std::string>> commands = {
    {"/item", {item, "Creates an item. Usage: /item <type> <id>"}},
    {"/load_npc", {load_npc, "Loads npc(s). TODO"}},
    {"/tp", {teleport, "Teleports a player or self. Usage: /tp <map_id> <x> <y> [client_id]"}},
    {"/die", {die, "Kills self"}},
    {"/dead", {die, "Kills self"}},
};

void help(Entity entity, const std::string& command) {
    auto client = getClient(entity);
    client->send(makePacket<ePacketType::PAKWC_WHISPER_CHAT>("", "help:"));
    if (command.size()) {
        client->send(makePacket<ePacketType::PAKWC_WHISPER_CHAT>("", command + " " + commands[command].second));
    } else {
        auto logger = Core::CLog::GetLogger(Core::log_type::SYSTEM).lock();
        for (const auto &[key, value] : commands) {
            std::string text;
            text = key + " " + value.second;
            logger->info("help: {}", text);
            client->send(makePacket<ePacketType::PAKWC_WHISPER_CHAT>("", text));
        }
    }
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

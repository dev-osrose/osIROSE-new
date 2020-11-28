#include "map.h"
#include <chrono>
#include "chat/normal_chat.h"
#include "chat/shout_chat.h"
#include "chat/whisper_chat.h"
#include "combat/combat.h"
#include "items/inventory.h"
#include "map/change_map.h"
#include "mouse/mouse_cmd.h"
std::literals::chrono_literals
Map::Map(uint16_t map_id) : map_id(map_id), is_running(true),
    lua_loader(*this, map_id, Core::Config::getInstance().mapServer().luaScript), loading(true) {
    logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();

    // load item lua
    const auto &itemDb = RoseCommon::ItemDatabase::getInstance();
    itemDb.store_lua([this](uint8_t type, uint16_t id, const std::string& lua) {
        lua_loader.load_lua_item(type, id, lua, [](Entity, int, int) {}, [](Entity, int, int) {});
    });

    register_dispatchers();

    // load npc/mob/warpgates/spawn points lua
    lua_loader.load_file(Core::Config::getInstance().mapServer().luaScript);
    loading = false;
}

void Map::register_dispatchers() {
    register_dispatcher(Chat::normal_chat);
    register_dispatcher(Chat::whisper_chat);
    register_dispatcher(Chat::whisper_chat_srv);
    register_dispatcher(Chat::shout_chat);
    register_dispatcher(Chat::announce_chat_srv);
    register_dispatcher(Map::change_map_request);
    register_dispatcher(Map::teleport_request);
    register_dispatcher(Mouse::mouse_cmd);
    register_dispatcher(Mouse::stop_moving);
    register_dispatcher(Combat::attack);
    register_dispatcher(Combat::hp_request);
    register_dispatcher(Combat::revive);
    register_dispatcher(Items::equip_item_packet);
    register_dispatcher(Items::drop_item_packet);

    register_dispatcher(Utils::transfer_to_char_server<RoseCommon::Packet::CliPartyReq>);
    register_dispatcher(Utils::transfer_to_char<RoseCommon::Packet::SrvPartyReq>);
    register_dispatcher(Utils::transfer_to_char_server<RoseCommon::Packet::CliPartyReply>);
    register_dispatcher(Utils::transfer_to_char<RoseCommon::Packet::SrvPartyReply>);
}

void Map::run() {
    using namespace std::literals::chrono_literals;
    while (is_running.load()) {
        // we dispatch all packets
        while (auto [ok, data] = packet_queue.pop_front(); ok) {
            dispatcher.dispatch(std::move(data.second), *this, entitySystem, data.first);
        }
        // then we run the systems
        // then we dispatch all events
        // then we sleep
        std::sleep(100ms);
    }
}

void Map::stop() {
    is_running.store(false);
}

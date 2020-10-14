#include "lua_loader.h"
#include "entity_system.h"
#include "throwassert.h"
#include "cmapclient.h"

LuaLoader::LuaLoader(EntitySystem& entitySystem, uint16_t map_id, const std::string& path)
    : entitySystem(entitySystem), logger(Core::CLog::GetLogger(Core::log_type::SCRIPTLOADER).lock()), map_id(map_id) {
    size_t pos = path.find_last_of('/');
    throw_assert(pos != std::string::npos, "Error, default path '" << path << "' is not valid");
    std::string base = path.substr(0, pos + 1);
    // check if needed: state.open_libraries();
    state.set_function("include", [this, base](std::string path) {
        load_file(base + path);
    });
}

void LuaLoader::load_file(const std::string& path) {
    try {
        // clear the eventual data from a previous load
        lua_db.delete_filename(path);

        // this is a reference, every copy is shallow so we
        // should be able to delete them without risk of segfaults or
        // undefined functions (if we have multiple stuff in the same env)
        sol::environment env{state, sol::create, state.globals()};
        // warp gates
        env.set_function("warp_gate", [this, env, path](std::string alias,
            int dest_map_id, float dest_x, float dest_y, float dest_z,
            int _map_id, float min_x, float min_y, float min_z,
            float max_x, float max_y, float max_z) {
            if (map_id != _map_id) return; // we skip anything that isn't needed
            int id = 0; // FIXME: get that from the lua
            auto entity = entitySystem.create_warpgate(alias, id, dest_map_id, dest_x, dest_y, dest_z, min_x, min_y, min_z, max_x, max_y, max_z);
            warps.register_lua(path, env, entity);
        });

        // npc positioning
        env.set_function("npc", [this, env, path](std::string npc_lua, int npc_id,
            int _map_id, float x, float y, float z, float angle) {
            if (map_id != _map_id) return;
            // create the npc and get the entity
            int quest_id = 0;
            if (!npc_lua.empty()) {
                quest_id = std::stoi(npc_lua);
            }
            auto entity = entitySystem.create_npc(quest_id, npc_id, map_id, x, y, z, angle);
            npcs.register_lua(path, env, entity);
            entitySystem.send_nearby_except_me(entity, CMapClient::create_srv_npc_char(entitySystem, entity));
        });

        // spawner
        env.set_function("mob", [this, env, path](std::string alias, int mob_id,
            int mob_count, int limit, int interval, int range,
            int _map_id, float x, float y, float z) {
            if (map_id != _map_id) return;
            auto entity = entitySystem.create_spawner(alias, mob_id, mob_count, limit, interval, range, _map_id, x, y, z);
            spawners.register_lua(path, env, entity);
        });

        // revive points
        env.set_function("revive_point", [this, env, path](int _map_id, float x, float y) {
            if (map_id != _map_id) return;
            auto entity = entitySystem.create_player_spawn(Component::PlayerSpawn::REVIVE_POINT, _map_id, x, y);
            player_spawns.register_lua(path, env, entity);
        });

        // start point
        env.set_function("start_point", [this, env, path](int _map_id, float x, float y) {
            if (map_id != _map_id) return;
            auto entity = entitySystem.create_player_spawn(Component::PlayerSpawn::START_POINT, _map_id, x, y);
            player_spawns.register_lua(path, env, entity);
        });

        // respawn point
        env.set_function("respawn_point", [this, env, path](int _map_id, float x, float y) {
            if (map_id != _map_id) return;
            auto entity = entitySystem.create_player_spawn(Component::PlayerSpawn::RESPAWN_POINT, _map_id, x, y);
            player_spawns.register_lua(path, env, entity);
        });

        // register npc (both mobs and npcs)
        env.set_function("registerNpc", [this, env, path](int npc_id, sol::table npc_data) {
            lua_db.store_lua(path, env, npc_id, npc_data);
        });

        state.script_file(path, env);
    } catch (const sol::error& e) {
        logger->error("Error (re)loading lua scripts '{}' : {}", path, e.what());
    }
}

void LuaLoader::load_lua_item(uint8_t type, uint16_t id, const std::string& lua,
                              std::function<void(Entity, int, int)>&& addBonusAttr,
                              std::function<void(Entity, int, int)>&& removeBonusAttr) {
    sol::environment env{state, sol::create, state.globals()};

    try {
        state.script(lua, env);
        items.register_lua(env, type, id, std::forward<decltype(addBonusAttr)>(addBonusAttr), std::forward<decltype(removeBonusAttr)>(removeBonusAttr));
    } catch (const sol::error& e) {
        logger->error("Error (re)loading item lua script: {}", e.what());
    }
}

std::weak_ptr<ItemLuaApi> LuaLoader::get_lua_item(uint8_t type, uint16_t id) {
    return items.get_lua_api(type, id);
}

std::weak_ptr<NpcLuaApi> LuaLoader::get_lua_npc(int npc_id) {
    return lua_db.get_lua_api(npc_id);
}

std::weak_ptr<LuaData> LuaLoader::get_data(int npc_id) const {
    return lua_db.get_data(npc_id);
}

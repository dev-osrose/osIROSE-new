#include "lua_loader.h"
#include "entity_system.h"
#include "throwassert.h"

LuaLoader::LuaLoader(EntitySystem& entitySystem, uint16_t map_id, const std::string& path)
    : entitySystem(entitySystem), logger(Core::CLog::GetLogger(Core::log_type::SCRIPTLOADER).lock()), map_id(map_id) {
    size_t pos = path.find_last_of('/');
    throw_assert(pos != std::string::npos, "Error, default path '" << path << "' is not valid");
    base = path.substr(0, pos + 1);
    // check if needed: state.open_libraries();
    state.set_function("include", [this, base](std::string path) {
        load_lua(base + path);
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
            int _map_id, float x, float y, float z, float angle,
            float x_scale, float y_scale, float z_scale) {
            if (map_id != _map_id) return; // we skip anything that isn't needed
            // create the warp and get the entity
            //warps.register_lua(path, env, entity);
        });

        // npc positioning
        env.set_function("npc", [this, env, path](std::string npc_lua, int npc_id,
            int _map_id, float x, float y, float z, float angle) {
            if (map_id != _map_id) return;
            // create the npc and get the entity
            // 
            //npcs.register_lua(path, env, entity);
        });

        // spawner
        env.set_function("mob", [this, env, path](std::string alias, int mob_id,
            int mob_count, int limit, int interval, int range,
            int _map_id, float x, float y, float z) {
            if (map_id != _map_id) return;
            // create the spawner and get the entity
            //spawners.register_lua(path, env, entity);
        });

        // revive points
        env.set_function("revive_point", [this, env, path](int _map_id, float x, float y) {
            if (map_id != _map_id) return;
            // do something here
        });

        // start point
        env.set_function("start_point", [this, env, path](int _map_id, float x, float y) {
            if (map_id != _map_id) return;
            // do something here
        });

        // respawn point
        env.set_function("respawn_point", [this, env, path](int _map_id, float x, float y) {
            if (map_id != _map_id) return;
            // do something here
        });

        // register npc (both mobs and npcs)
        env.set_function("registerNpc", [this, env, path](int npc_id, sol::table npc_data) {
            if (map_id != _map_id) return;
            lua_db.store_lua(path, env, npc_id, npc_data);
        });

        state.script_file(path, env);
    } catch (const sol::error& e) {
        logger->error("Error (re)loading lua scripts '{}' : {}", path, e.what());
    }
}

void LuaLoader::load_lua_item(uint8_t type, uint16_t id, const std::string& lua) {
    sol::environment env{state, sol::create, state.globals()};

    try {
        state.script(lua, env);
        items.register_lua(env, type, id);
    } catch (const sol::error& e) {
        logger->error("Error (re)loading item lua script: {}", e.what());
    }
}

std::weak_ptr<ItemLuaApi> get_lua_item(uint8_t type, uint16_t id) const {
    return items.get_lua_api(type, id);
}
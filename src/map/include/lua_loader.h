#pragma once

#include "logconsole.h"
#include "lua_db.h"
#include "lua_storage.h"
#include "lua_item.h"

#include <sol.hpp>

#include <string>

class EntitySystem;

class LuaLoader {
    public:
        LuaLoader(EntitySystem&, uint16_t map_id, const std::string& path);

        void load_lua_item(uint8_t type, uint16_t id, const std::string& lua,
                           std::function<void(Entity, int, int)>&& addBonusAttr,
                           std::function<void(Entity, int, int)>&& removeBonusAttr);
        std::weak_ptr<ItemLuaApi> get_lua_item(uint8_t type, uint16_t id);

        std::weak_ptr<NpcLuaApi> get_lua_npc(int npc_id);
        std::weak_ptr<LuaData> get_data(int npc_id) const;
    
        void load_file(const std::string& path); // relative path

    private:
        EntitySystem& entitySystem;
        std::shared_ptr<spdlog::logger> logger;
        std::string base_path;
        uint16_t map_id;
        sol::state state;
        
        LuaDb lua_db;
        LuaStorage npcs;
        LuaStorage spawners;
        LuaStorage warps;
        LuaStorage points;
        LuaStorage player_spawns;
        LuaItem items;
};

#pragma once

#include "logconsole.h"
#include "lua_db.h"

#include <sol.hpp>

#include <string>

class EntitySystem;

class LuaLoader {
    public:
        LuaLoader(EntitySystem&, uint16_t map_id, const std::string& path);
    
    protected:
        void load_file(const std::string& path); // relative path

    private:
        EntitySystem& entitySystem;
        std::shared_ptr<spdlog::logger> logger;
        std::string base_path;
        uint16_t map_id;
        sol::state state;
        
        LuaDb lua_db;
};
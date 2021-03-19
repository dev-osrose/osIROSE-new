#pragma once

#include <sol.hpp>
#include <string>
#include <unordered_map>
#include <vector>

#include "entity.h"
#include "lua_api.h"

// stores all entities that are loaded from lua
class LuaStorage {
    public:
        void register_lua(const std::string& filename, sol::environment env, Entity);

        void delete_entity(Entity entity);
        std::vector<Entity> delete_filename(const std::string& filename);

    private:
        struct Data {
            std::string filename;
            sol::environment env;
        };

        std::unordered_map<Entity, Data> data;
        std::unordered_multimap<std::string, Entity> filename_to_entity;
};

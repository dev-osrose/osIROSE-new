#pragma once

#include <sol.hpp>
#include <string>
#include <unordered_map>
#include <unordered_multimap>
#include <vector>

#include "dataconsts.h"
#include "lua_api.h"

// stores all entities that are loaded from lua
class LuaStorage {
    public:
        void register_lua(const std::string& filename, sol::environment env, RoseCommon::Entity);

        void delete_entity(RoseCommon::Entity entity);
        std::vector<RoseCommon::Entity> delete_filename(const std::string& filename);

    private:
        struct Data {
            std::string filename;
            sol::environment env;
        };

        std::unordered_map<RoseCommon::Entity, Data> data;
        std::unordered_multimap<std::string, RoseCommon::Entity> filename_to_entity;
};
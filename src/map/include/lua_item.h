#pragma once

#include <sol.hpp>
#include <string>
#include <unordered_map>
#include <tuple>

#include "hash.h"
#include "lua_api.h"

// stores all item related lua
class LuaItem {
    public:
        void register_lua(sol::environment env, uint8_t type, uint16_t id,
                          std::function<void(Entity, int, int)>&& addBonusAttr,
                          std::function<void(Entity, int, int)>&& removeBonusAttr);

        void delete_item(uint8_t type, uint16_t id);

        std::weak_ptr<ItemLuaApi> get_lua_api(uint8_t type, uint16_t id);

    private:
        struct Data {
            std::shared_ptr<ItemLuaApi> api;
            sol::environment env;
        };

        using key_t = std::tuple<uint8_t, uint16_t>;
        std::unordered_map<key_t, Data, Core::tuple_hash> data;
};

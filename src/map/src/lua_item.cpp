#include "lua_item.h"

void LuaItem::register_lua(sol::environment env, uint8_t type, uint16_t id) {
    Data d;
    d.api = std::make_shared<ItemLuaApi>(env);
    d.env = env;
    data.emplace({type, id}, d);
}

void LuaItem::delete_item(uint8_t type, uint16_t id) {
    data.erase({type, id});
}

std::weak_ptr<ItemLuaApi> LuaItem::get_lua_api(uint8_t type, uint16_t id) {
    if (auto it = data.find({type, id}); it != data.end()) {
        return it->second.api;
    }
    return {};
}
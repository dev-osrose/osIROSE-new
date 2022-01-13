#include "lua_item.h"

void LuaItem::register_lua(sol::environment env, uint8_t type, uint16_t id,
                           std::function<void(Entity, int, int)>&& addBonusAttr,
                           std::function<void(Entity, int, int)>&& removeBonusAttr) {
    Data d;
    d.api = std::make_shared<ItemLuaApi>(env);
    d.api->register_add_bonus_attr(std::forward<decltype(addBonusAttr)>(addBonusAttr));
    d.api->register_remove_bonus_attr(std::forward<decltype(removeBonusAttr)>(removeBonusAttr));
    d.env = env;
    data.emplace(std::tuple{type, id}, d);
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

#pragma once;

#include <memory>

class ItemLuaApi;
class NpcLuaApi;

struct ItemLua {
    std::weak_ptr<ItemLuaApi> api;
};

struct NpcLua {
    std::weak_ptr<NpcLuaApi> api;
};
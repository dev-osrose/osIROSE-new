#pragma once

#include <memory>

class ItemLuaApi;
class NpcLuaApi;
class LuaData;

namespace Component {
struct ItemLua {
    std::weak_ptr<ItemLuaApi> api;
};

struct NpcLua {
    std::weak_ptr<NpcLuaApi> api;
    std::weak_ptr<LuaData> data;
};
}

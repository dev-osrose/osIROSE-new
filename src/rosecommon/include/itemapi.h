#pragma once

#include "luaapi.h"

namespace RoseCommon {

class ItemAPI : public LuaAPI {
  public:
    ItemAPI() = default;
    virtual ~ItemAPI() = default;
    ItemAPI(sol::environment&& env);

    void onInit() { safeLuaCall<bool>("OnInit"); }
    void onDelete() { safeLuaCall<bool>("OnDelete"); }
    bool onEquip(void* entity) { return safeLuaCall<bool>("OnEquip", entity).value_or(false); }
    bool onUnequip(void* entity) { return safeLuaCall<bool>("OnUnequip", entity).value_or(false); }
    bool onDrop(void* entity) { return safeLuaCall<bool>("OnDrop", entity).value_or(false); }
    bool onPickup(void* entity) { return safeLuaCall<bool>("OnPickup", entity).value_or(false); }
    bool onUse(void* entity) { return safeLuaCall<bool>("OnUse", entity).value_or(false); }
};

}

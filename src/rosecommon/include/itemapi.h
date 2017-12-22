#pragma once

#include "luaapi.h"

namespace RoseCommon {

class ItemAPI : public LuaAPI {
  public:
    ItemAPI() = default;
    ItemAPI(sol::environment&& env);

    void onInit() { env_["onInit"](); }
    void onDelete() { env_["onDelete"](); }
    bool onEquip(void* entity) { return env_["onEquip"](entity); }
    bool onUnequip(void* entity) { return env_["onUnEquip"](entity); }
    bool onDrop(void* entity) { return env_["onDrop"](entity); }
    bool onPickup(void* entity) { return env_["onPickup"](entity); }
    bool onUse(void* entity) { return env_["onUse"](entity); }
};

}

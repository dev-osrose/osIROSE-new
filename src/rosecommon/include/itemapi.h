#pragma once

#include "luaapi.h"

namespace RoseCommon {

class ItemAPI : public LuaAPI {
  public:
    ItemAPI() = default;
    virtual ~ItemAPI() { onDelete(); }
    ItemAPI(sol::environment&& env);

    void onInit() { env_["OnInit"](); }
    void onDelete() { env_["OnDelete"](); }
    bool onEquip(void* entity) { return env_["OnEquip"](entity); }
    bool onUnequip(void* entity) { return env_["OnUnEquip"](entity); }
    bool onDrop(void* entity) { return env_["OnDrop"](entity); }
    bool onPickup(void* entity) { return env_["OnPickup"](entity); }
    bool onUse(void* entity) { return env_["OnUse"](entity); }
};

}

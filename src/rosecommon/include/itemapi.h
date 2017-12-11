#pragma once

#include "luaapi.h"

namespace RoseCommon {

class ItemAPI : public LuaAPI {
  public:
    ItemAPI() = default;
    ItemAPI(EntitySystem& es, sol::environment&& env);

    void onInit() { env_["onInit"](); }
    void onDelete() { env_["onDelete"](); }
    bool onEquip(uint32_t id) { return env_["onEquip"](id); }
    bool onUnequip(uint32_t id) { return env_["onUnEquip"](id); }
    bool onDrop(uint32_t id) { return env_["onDrop"](id); }
    bool onPickup(uint32_t id) { return env_["onPickup"](id); }
    bool onUse(uint32_t id) { return env_["onUse"](id); }
};

}

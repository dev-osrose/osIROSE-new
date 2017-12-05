#pragma once

#include "luaapi.h"

namespace RoseCommon {

class ItemAPI : public LuaAPI {
  public:
    ItemAPI() = default;
    ItemAPI(sol::environment&& env) : LuaAPI(std::move(env)) {
      // build the C++/lua connectors here
      /*env_.set_function("getAttr", [this](Entity e, std::string attr) {
        logger_->info("getAttr called for entity {} and attr {}", getId(e), attr);
      });*/
    }

    /*void onInit() { env_["onInit"](); }
    void onDelete() { env_["onDelete"](); }
    bool onEquip(Entity e) { return env_["onEquip"](e); } // character's entity
    bool onUnequip(Entity e) { return env_["onUnequip"](e); } // character's entity
    bool onDrop(Entity e) { return env_["onDrop"](e); } // self's entity
    bool onPickup(Entity e) { return env_["onPickup"](e); } // character's entity
    bool onUse(Entity e) { return env_["onUse"](e); } // character's entity
    */
};

}

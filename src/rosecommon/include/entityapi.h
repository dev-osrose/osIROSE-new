#pragma once

#include "luaapi.h"

namespace RoseCommon {

class EntityAPI : public LuaAPI {
  public:
    EntityAPI() = default;
    EntityAPI(sol::environment&& env) : LuaAPI(std::move(env)) {
      // setup the C++/lua connectors
      env_.set_function("display", [this] (std::string data) {
         logger_->warn("lua display call: {}", data);
      });
    }

    void onInit() { auto f = env_["onInit"]; f(); }
};

}

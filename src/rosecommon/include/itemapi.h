#pragma once

#include "luaapi.h"

namespace RoseCommon {

class ItemAPI : public LuaAPI {
  public:
    ItemAPI() = default;
    ItemAPI(sol::environment&& env) : LuaAPI(std::move(env)) {
      // build the C++/lua connectors here
    }
};

}

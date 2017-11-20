#pragma once

#include <sol/environment.hpp>

namespace RoseCommon {

class LuaAPI {
  public:
    LuaAPI(sol::environment&& luaEnv) : luaEnv_(std::move(luaEnv) {}
    
    virtual void update(double dt) { (void)dt; }
    virtual void recv() {}
    
  protected:
    sol::environment luaEnv_;
}

}

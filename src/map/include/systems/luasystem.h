#pragma once

#include "system.h"
#include "throwassert.h"

#include <sol.hpp>

#include <vector>
#include <algorithm>

namespace Systems {

class LuaSystem : public System {
    public:
        LuaSystem(SystemManager &manager) : System(manager) {}
        virtual ~LuaSystem() = default;

        void loadScript(Entity e, const std::string& luaScript) {
          auto lua = e.component<Lua>();
          throw_assert(lua, "The entity doesn't have a lua table");
          if (!lua->env_)
            lua->env_ = std::make_unique<sol::environment>(state_, sol::create);
          lua->env_->set_function("testCpp",
                                [this, e](std::string data) {
                                    auto basic = e.component<BasicInfo>();
                                    logger_->warn("test lua: {}, entity: {}", data, basic->name_);
                              });
          state_.script(luaScript, *lua->env_);
        }

        void callLuaFunction(Entity e, const std::string name) {
          auto lua = e.component<Lua>();
          throw_assert(lua && lua->env_, "The entity doesn't have a lua table");
          auto &env = *lua->env_;
          sol::function func = env[name];
          func();
        }

        void unregisterEntity(Entity e) {
          auto it = std::find(callbacks_.begin(), callbacks_.end(), Callback{e});
            if (it != callbacks_.end())
              callbacks_.erase(it);
        }

        virtual void update(EntityManager&, double dt) {
            for (auto &it : callbacks_) {
                it.dt += dt;
                if (it.dt >= it.timeout) {
                    it.dt = 0.f;
                }
            }
        }

    private:
        sol::state state_;

        struct Callback {
            Entity e;
            std::string name{};
            double timeout = 0;
            double dt = 0;

            bool operator==(const Callback& c) const { return e == c.e; }
        };
        std::vector<Callback> callbacks_;
};

}

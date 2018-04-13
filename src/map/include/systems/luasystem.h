#pragma once

#include "system.h"
#include "throwassert.h"
#include "function_traits.h"

#include <sol.hpp>

#include <vector>
#include <algorithm>

namespace Systems {

class LuaSystem : public System {
    public:
        LuaSystem(SystemManager &manager) : System(manager) {
            // FIXME: this loads every library available. We should check if all of them are useful
            state_.open_libraries();
        }
        virtual ~LuaSystem() = default;

        void loadScript(Entity e, const std::string& luaScript) {
          auto lua = e.component<RoseCommon::EntityAPI>();
          throw_assert(lua, "The entity doesn't have a lua table");
          auto loadedLua = loadScript<RoseCommon::EntityAPI>(luaScript);
          if (!loadedLua) {
              return;
          }
          *lua = std::move(loadedLua.value());
        }

        template <typename LuaAPI>
        LuaAPI loadScript(const std::string& luaScript) {
          sol::environment env{state_, sol::create, state_.globals()};
          LuaAPI lua{std::move(env)};
          try {
              state_.script(luaScript, lua.getEnv());
          } catch (const sol::error& e) {
              logger_->error("Lua error while loading the script: {}", e.what());
              return {};
          }
          return lua;
        }

    virtual void update(EntityManager&, double) {}

        /*void unregisterEntity(Entity e) {
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
        }*/

    private:
        sol::state state_;

        /*struct Callback {
            Entity e;
            std::string name{};
            double timeout = 0;
            double dt = 0;

            bool operator==(const Callback& c) const { return e == c.e; }
        };
        std::vector<Callback> callbacks_;*/
};

}

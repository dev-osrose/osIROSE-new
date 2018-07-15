#pragma once

#include "system.h"
#include "throwassert.h"
#include "function_traits.h"

#include <sol.hpp>

#include <vector>
#include <algorithm>
#include <optional>

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
        std::optional<LuaAPI> loadScript(const std::string& luaScript) {
          sol::environment env{state_, sol::create, state_.globals()};
          LuaAPI lua{std::move(env)};
          try {
              state_.script(luaScript, lua.getEnv());
          } catch (const sol::error& e) {
              logger_->error("Lua error while loading the script: {}", e.what());
              return {};
          }
          return std::make_optional(std::move(lua));
        }

    virtual void update(EntityManager&, std::chrono::milliseconds) override {}

    private:
        sol::state state_;
};

}

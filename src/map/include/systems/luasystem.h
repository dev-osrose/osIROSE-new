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
          *lua = loadScript<RoseCommon::EntityAPI>(luaScript);
        }

        template <typename LuaAPI>
        LuaAPI loadScript(const std::string& luaScript) {
          sol::environment env{state_, sol::create, state_.globals()};
          LuaAPI lua{std::move(env)};
          state_.script(luaScript, lua.getEnv());
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
            
<<<<<<< HEAD

=======
        static constexpr auto getFunctionName(luaFunctions func) {
            switch (func) {
                case luaFunctions::onInit: return "onInit";
                case luaFunctions::onCreate: return "onCreate";
                case luaFunctions::onRemove: return "onRemove";
                case luaFunctions::onEquip: return "onEquip";
                case luaFunctions::onUnEquip: return "onUnEquip";
                case luaFunctions::onDrop: return "onDrop";
                case luaFunctions::onPickup: return "onPickup";
                case luaFunctions::onUse: return "onUse";
                default: return "";
            }
        }
    
        struct voidvoid { using type = void(); };
        struct boolvoid { using type = bool(); };
    
        template <luaFunctions func>
        static constexpr auto getFunctionType() {
            if constexpr (func == luaFunctions::onInit) return voidvoid{};
            else if constexpr (func == luaFunctions::onCreate) return voidvoid{};
            else if constexpr (func == luaFunctions::onRemove) return voidvoid{};
            else if constexpr (func == luaFunctions::onEquip) return boolvoid{};
            else if constexpr (func == luaFunctions::onUnEquip) return boolvoid{};
            else if constexpr (func == luaFunctions::onDrop) return boolvoid{};
            else if constexpr (func == luaFunctions::onPickup) return voidvoid{};
            else if constexpr (func == luaFunctions::onUse) return voidvoid{};
            else return voidvoid{};
        }
>>>>>>> 7a6728df... Update luasystem.h
};

}

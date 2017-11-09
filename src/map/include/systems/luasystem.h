#pragma once

#include "system.h"
#include "throwassert.h"
#include "function_trait.h"

#include <sol.hpp>

#include <vector>
#include <algorithm>

namespace Systems {

class LuaSystem : public System {
    public:
        enum class luaFunctions {
            onInit,
            onCreate,
            onRemove,
            onEquip,
            onUnEquip,
            onDrop,
            onPickup,
            onUse,
            other
        };
    
        LuaSystem(SystemManager &manager) : System(manager) {}
        virtual ~LuaSystem() = default;

        void loadScript(Entity e, const std::string& luaScript) {
          auto lua = e.component<Lua>();
          throw_assert(lua, "The entity doesn't have a lua table");
          loadScript(*lua, luaScript);
        }
    
        void loadScript(Lua& luaEnv, const std::string& luaScript) {
            if (!luaEnv.env_)
                luaEnv.env_ = std::make_unique<sol::environment>(state_, sol::create);
            luaEnv.env_->set_function("testCpp",
                                [this, e](std::string data) {
                                    auto basic = e.component<BasicInfo>();
                                    logger_->warn("test lua: {}, entity: {}", data, basic->name_);
                              });
          state_.script(luaScript, *luaEnv.env_);
        }

        void callLuaFunction(Entity e, luaFunctions function) {
          auto lua = e.component<Lua>();
          throw_assert(lua && lua->env_, "The entity doesn't have a lua table");
          callLuaFunction(*lua, function);
        }
    
        template <luaFunctions func, typename... Args>
        auto callLuaFunction(Lua& luaEnv, Args... args) {
            auto& env = *luaEnv.env_;
            sol::function func = env[getFunctionName(func)];
            using FuncType = decltype(getFunctionType<func>())::type;
            static_assert(std::is_same<FuncType::arguments, decltype(std::make_tuple(args...))>::value, "Incorrect parameters for the lua function");
            return func(args...);
        }

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
            
        static constexpr auto getFunctionName(luaFunctions func) {
            switch (func) {
                case onInit: return "onInit";
                case onCreate: return "onCreate";
                case onRemove: return "onRemove";
                case onEquip: return "onEquip";
                case onUnEquip: return "onUnEquip";
                case onDrop: return "onDrop";
                case onPickup: return "onPickup";
                case onUse: return "onUse";
                default: return "";
            }
        }
    
        struct voidvoid { using type = void(*)(); };
        struct boolvoid { using type = bool(*)(); };
    
        template <luaFunctions func>
        static constexpr auto getFunctionType() {
            if constexpr (func == onInit) return voidvoid{};
            else if constexpr (func == onCreate) return voidvoid{};
            else if constexpr (func == onRemove) return voidvoid{};
            else if constexpr (func == onEquip) return boolvoid{};
            else if constexpr (func == onUnEquip) return boolvoid{};
            else if constexpr (func == onDrop) return boolvoid{};
            else if constexpr (func == onPickup) return voidvoid{};
            else if constexpr (func == onUse) return voidvoid{};
            else voidvoid{};
        }
};

}

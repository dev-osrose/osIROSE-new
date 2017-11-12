#pragma once

#include "system.h"
#include "throwassert.h"
#include "function_traits.h"
#include "luaapi.h"
#include "entityapi.h"
#include "itemapi.h"

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
            onUse
        };
    
        LuaSystem(SystemManager &manager) : System(manager) {}
        virtual ~LuaSystem() = default;

        void loadScript(Entity e, const std::string& luaScript) {
          auto lua = e.component<Lua>();
          throw_assert(lua, "The entity doesn't have a lua table");
          loadScript(*lua, luaScript, [e]() { return std::unique_ptr<LuaApi>{new EntityApi(e)}; });
        }

        void loadScript(RoseCommon::Item& e, const std::string& luaScript) {
            loadScript(e.lua_, luaScript, [&e]() { return std::unique_ptr<LuaApi>{new ItemApi(e)}; });
        }
 
        template <typename Lambda>
        void loadScript(Lua& luaEnv, const std::string& luaScript, Lambda&& apiBuilder) {
            if (!luaEnv.env_)
                luaEnv.env_ = std::make_unique<sol::environment>(state_, sol::create);
            if (!luaEnv.env_)
                luaEnv.api_ = apiBuilder();
            luaEnv.api_->setupApi(*luaEnv.env_);
            luaEnv.env_->set_function("display", [this] (std::string data) {
                    logger_->warn("lua display call: {}", data);
                    });
          state_.script(luaScript, *luaEnv.env_);
        }

        template <luaFunctions func, typename... Args>
        void callLuaFunction(Entity e, Args... args) {
          auto lua = e.component<Lua>();
          throw_assert(lua && lua->env_, "The entity doesn't have a lua table");
          callLuaFunction<func>(*lua, args...);
        }
    
        template <luaFunctions func, typename... Args>
        auto callLuaFunction(Lua& luaEnv, Args... args);
    
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


namespace {
template <LuaSystem::luaFunctions> struct LuaFunction {};
template <> struct LuaFunction<LuaSystem::luaFunctions::onInit> { static constexpr const char*const name = "onInit"; using type = void(); };
template <> struct LuaFunction<LuaSystem::luaFunctions::onCreate> { static constexpr const char*const name = "onCreate"; using type = void(); };
template <> struct LuaFunction<LuaSystem::luaFunctions::onRemove> { static constexpr const char*const name = "onRemove"; using type = void(); };
template <> struct LuaFunction<LuaSystem::luaFunctions::onEquip> { static constexpr const char*const name = "onEquip"; using type = bool(); };
template <> struct LuaFunction<LuaSystem::luaFunctions::onUnEquip> { static constexpr const char*const name = "onUnEquip"; using type = bool(); };
template <> struct LuaFunction<LuaSystem::luaFunctions::onDrop> { static constexpr const char*const name = "onDrop"; using type = void(); };
template <> struct LuaFunction<LuaSystem::luaFunctions::onPickup> { static constexpr const char*const name = "onPickup"; using type = void(); };
template <> struct LuaFunction<LuaSystem::luaFunctions::onUse> { static constexpr const char*const name = "onUse"; using type = void(); };
}

template <LuaSystem::luaFunctions func, typename... Args>
auto LuaSystem::callLuaFunction(Lua& luaEnv, Args... args) {
    auto& env = *luaEnv.env_;
    using Func = LuaFunction<func>;
    sol::function f = env[Func::name];
    using FuncType = function_traits<typename Func::type>;
    static_assert(std::is_same<typename FuncType::arguments, std::tuple<Args...>>::value, "Incorrect parameters for the lua function");
    return static_cast<typename FuncType::return_type>(f(args...));
}
}

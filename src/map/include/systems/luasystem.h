#pragma once

#include "system.h"
#include "throwassert.h"

#include <sol.hpp>

#include <unordered_set>

namespace Systems {

class LuaSystem : public System {
    public:
        LuaSystem(SystemManager &manager) : System(manager) {}
        virtual ~LuaSystem() = default;

        void registerLuaUpdate(Entity e, const std::string& luaFunc, double dt) {
            auto *data = e.component<LuaData>();
            throw_assert(data == nullptr, "The entity tried to register a lua function but doesn't have a lua component");
            if (!data->env_)
                data->env_ = std::make_unique<sol::environment>(state_, sol::create);
            callbacks_.insert({e, state_.load(luaFunc, *data->env_), dt});
        }
    
        template <typename... Args>
        void callLuaFunction(Entity e, const std::string& luaFunc, Args... args) {
            auto *data = e.component<LuaData>();
            sol::function f;
            if (!data || !data->env_)
                f = state_.load(luaFunc);
            else
                f = state_.load(luaFunc, *data->env);
            f(e, args...);
        }
    
        virtual void update(EntityManager&, double dt) {
            for (auto &it : callbacks_) {
                it.dt += dt;
                if (it.dt >= timeout) {
                    func(it.e, it.dt);
                    it.dt = 0.f;
                }
            }
        }
    
    private:
        sol::state state_;
        
        struct Callback {
            Entity e;
            sol::function func;
            double timeout;
            double dt = 0;
        };
        std::unordered_set<Callback> callbacks_;
};

}

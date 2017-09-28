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

        void registerLuaUpdate(Entity e, std::string_view name, std::string_view luaFunc, double dt) {
            auto data = e.component<LuaData>();
            throw_assert(data == nullptr, "The entity tried to register a lua function but doesn't have a lua component");
            if (!data->env_)
                data->env_ = std::make_unique<sol::environment>(state_, sol::create);
            state_.script(luaFunc, *data->env_);
            callbacks_.push_back({e, name, dt});
        }

        template <typename... Args>
        void callLuaFunction(Entity e, std::string_view luaFunc, Args&&... args) {
            auto data = e.component<LuaData>();
            sol::function f;
            if (!data || !data->env_)
                f = state_.script(luaFunc);
            else
              f = state_.script(luaFunc, *data->env_);
            f(e, std::move(args)...);
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
                  //auto data = it.e.component<LuaData>();
                    auto func = state_[it.name];
                    func(it.e, it.dt);
                    it.dt = 0.f;
                }
            }
        }

    private:
        sol::state state_;

        struct Callback {
            Entity e;
            std::string_view name{};
            double timeout = 0;
            double dt = 0;

            bool operator==(const Callback& c) const { return e == c.e; }
        };
        std::vector<Callback> callbacks_;
};

}

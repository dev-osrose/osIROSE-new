#pragma once

#include "system.h"

#include <sol.hpp>

namespace Systems {

class LuaSystem : public System {
    public:
        LuaSystem(SystemManager &manager) : System(manager) {}
        virtual ~LuaSystem() = default;

        virtual void update(EntityManager &es, double dt) {}
    
    private:
        sol::state state_;
};

}

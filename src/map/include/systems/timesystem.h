#pragma once

#include "system.h"

namespace Systems {

class TimeSystem : public System {
    public:
        TimeSystem(SystemManager &manager) : System(manager) {}
        virtual ~TimeSystem() = default;

        virtual void update(EntityManager &es, double dt);
};

}

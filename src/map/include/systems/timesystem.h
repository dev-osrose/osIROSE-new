#pragma once

#include "system.h"

namespace Systems {

class TimeSystem : public System {
    public:
        TimeSystem(EntityManager&, SystemManager&) {}
        virtual ~TimeSystem() = default;

        virtual void update(EntityManager &es, double dt);
};

}

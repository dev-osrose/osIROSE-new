#pragma once

#include "entityComponents.h"
#include "systemmanager.h"
#include "packets.h"

namespace Systems {

class System {
    public:
        virtual ~System() = default;

        virtual void update(EntityManager&, double dt) = 0;
};

}

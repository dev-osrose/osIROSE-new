#pragma once

#include "system.h"

namespace Systems {

class UpdateSystem : public System {
    public:
        UpdateSystem(SystemManager&) {}
        virtual ~UpdateSystem() = default;

        virtual void update(EntityManager &es, double);

        void calculateSpeed(Entity entity);

        void calculateAtkSpeed(Entity entity);

        void calculateCommand(Entity entity);
};

}

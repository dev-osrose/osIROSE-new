
#ifndef _ENTITYSYSTEM_H_
# define _ENTITYSYSTEM_H_

#include "entitySystems.h"
#include <map>

class EntitySystem {
    public:
        EntitySystem() : systemManager_(entityManager_) {
        }

        void update(double dt) {
            systemManager_.update(dt);
        }

        Entity create() {
            return entityManager_.create();
        }

    private:
        EntityManager entityManager_;
        SystemManager systemManager_;
};

#endif /* !_ENTITYSYSTEM_H_ */

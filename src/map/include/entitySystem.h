
#ifndef _ENTITYSYSTEM_H_
# define _ENTITYSYSTEM_H_

#include "entitySystems.h"

class EntitySystem {
    public:
        EntitySystem() : systemManager_(entityManager_) {
            systemManager_.add<MovementSystem>();
        }

        void update(double dt) {
            systemManager_.update(dt);
        }

        Entity create() {
            return entityManager_.create();
        }

        template <typename T>
        T& get() {
            return *systemManager_.get<T>();
        }

    private:
        EntityManager entityManager_;
        SystemManager systemManager_;
};

namespace EntitySystemManipulators {

void move(std::shared_ptr<EntitySystem> es, Entity e, float x, float y) ;
}

#endif /* !_ENTITYSYSTEM_H_ */

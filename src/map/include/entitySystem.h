
#ifndef _ENTITYSYSTEM_H_
# define _ENTITYSYSTEM_H_

#include "entitySystems.h"
#include <functional>

class EntitySystem {
    public:
        EntitySystem() : systemManager_(entityManager_) {
            // TODO : use on_component_removed for Destination
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

        Entity loadCharacter(uint32_t charId, bool platinium);
        void saveCharacter(uint32_t  charId, Entity entity);

        template <typename ...T>
        void process(std::function<void(Entity)> func) {
            for (Entity entity : entityManager_.entities_with_components<T...>())
                func(entity);
        }

    private:
        EntityManager entityManager_;
        SystemManager systemManager_;
};

#endif /* !_ENTITYSYSTEM_H_ */

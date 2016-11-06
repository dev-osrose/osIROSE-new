
#ifndef _ENTITYSYSTEM_H_
# define _ENTITYSYSTEM_H_

#include "entitySystems.h"
#include <functional>
#include <vector>
#include <type_traits>

class EntitySystem {
    public:
        EntitySystem() : systemManager_(entityManager_) {
            // TODO : use on_component_removed for Destination
            systemManager_.add<MovementSystem>();
            systemManager_.add<UpdateComponents>();
        }

        void update(double dt) {
            systemManager_.update(dt);
            for (auto it : toDestroy_)
                if (it)
                    it.destroy();
            toDestroy_.clear();
        }

        void destroy(Entity entity) {
            if (!entity)
                return;
            toDestroy_.push_back(entity);
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
        void processEntities(std::function<bool(Entity)>&& func) {
            for (Entity entity : entityManager_.entities_with_components<T...>())
                if (!func(entity))
                    return;
        }

    private:
        EntityManager entityManager_;
        SystemManager systemManager_;
        std::vector<Entity> toDestroy_;
};

#endif /* !_ENTITYSYSTEM_H_ */

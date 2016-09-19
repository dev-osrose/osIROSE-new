
#ifndef _ENTITYSYSTEMS_H_
# define _ENTITYSYSTEMS_H_

#include "entityComponents.h"

#define equals(a, b, precision) (((a + precision) > (b - precision)) && ((a - precision) < (b + precision)))

using EntityManager = entityx::EntityX<GameComponents, entityx::ColumnStorage<GameComponents>>;
template <typename T>
using Component = EntityManager::Component<T>;
using Entity = EntityManager::Entity;

class System {
    public:
        virtual void update(EntityManager&, float dt) = 0;
};

class MovementSystem : public System {
    public:
        virtual void update(EntityManager &es, float dt) {
            Component<Position> position;
            Component<Destination> destination;
            for (Entity entity : es.entities_with_components(position, destination)) {
                int dx = 0, dy = 0;
                if (!equals(position->x, destination->x, 0.1) && position->x < destination->x)
                    dx = -1;
                else if (!equals(position->x, destination->x, 0.1) && position->x > destination->x)
                    dx = 1;
                if (!equals(position->y, destination->y, 0.1) && position->y < destination->y)
                    dy = -1;
                else if (!equals(position->y, destination->y, 0.1) && position->y > destination->y)
                    dy = 1;
                position->x += dx * 1 * dt; // TODO : compute speed
                position->y += dy * 1 * dt; // TODO : compute speed
                if (equals(position->x, destination->x, 0.1) && equals(position->y, destination->y))
                    entity.remove<Destination>();
            }
        }

        void move(entity, float x, float y) {
            // TODO
        }
};

#endif /* !_ENTITYSYSTEMS_H_ */

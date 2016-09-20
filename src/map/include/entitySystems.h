
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
                if (!equals(position->x_, destination->x_, 0.1) && position->x_ < destination->x_)
                    dx = -1;
                else if (!equals(position->x_, destination->x_, 0.1) && position->x_ > destination->x_)
                    dx = 1;
                if (!equals(position->y_, destination->y_, 0.1) && position->y_ < destination->y_)
                    dy = -1;
                else if (!equals(position->y_, destination->y_, 0.1) && position->y_ > destination->y_)
                    dy = 1;
                position->x_ += dx * 1 * dt; // TODO : compute speed
                position->y_ += dy * 1 * dt; // TODO : compute speed
                if (equals(position->x_, destination->x_, 0.1) && equals(position->y_, destination->y_, 0.1))
                    entity.remove<Destination>();
            }
        }

        void move(Entity entity, float x, float y) {
            (void)entity;
            (void)x;
            (void)y;
            // TODO
        }
};

#endif /* !_ENTITYSYSTEMS_H_ */


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
        virtual ~System() {}

        virtual void update(EntityManager&, double dt) = 0;
};

class MovementSystem : public System {
    public:
        virtual ~MovementSystem() {}

        virtual void update(EntityManager &es, double dt) {
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

class SystemManager {
    public:
        SystemManager(EntityManager &es) : entityManager_(es) {}

        template <typename T, typename... Args>
        T& add(Args&&... args) {
            systems_.emplace_back(new T(std::forward<Args>(args)...));
            return *dynamic_cast<T*>(systems_.back().get());
        }

        template <typename T>
        void remove() {
            for (auto it = systems_.begin(); it != systems_.end(); ++it) {
                if (dynamic_cast<T*>(it->get())) {
                    systems_.erase(it);
                    return;
                }
            }
        }

        template <typename T>
        T& get() {
            for (auto &it : systems_) {
                T *tmp;
                if ((tmp = dynamic_cast<T*>(it.get())))
                    return *tmp;
            }
        }

        void update(double dt) {
            for (auto &it : systems_)
                it->update(entityManager_, dt);
        }

    private:
        EntityManager &entityManager_;
        std::vector<std::unique_ptr<System>> systems_;
};

#endif /* !_ENTITYSYSTEMS_H_ */

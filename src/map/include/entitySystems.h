
#ifndef _ENTITYSYSTEMS_H_
# define _ENTITYSYSTEMS_H_

#include "entityComponents.h"
#include <cmath>

class EntitySystem;

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
            Component<AdvancedInfo> advanced;
            for (Entity entity : es.entities_with_components(position, destination, advanced)) {
                float dx = destination->x_ - position->x_;
                float dy = destination->y_ - position->y_;
                float distance = std::sqrt(dx * dx + dy * dy);
                float speed = advanced->runSpeed_;
                float ntime = distance / speed;
                if (ntime <= dt || distance == 0) {
                    position->x_ = destination->x_;
                    position->y_ = destination->y_;
                    entity.remove<Destination>();
                } else {
                    position->x_ += dx * dt / ntime;
                    position->y_ += dy * dt / ntime;
                }
            }
        }

        void move(Entity entity, float x, float y) {
            if (!entity)
                return;
            auto dest = entity.component<Destination>();
            if (dest) {
                dest->x_ = x;
                dest->y_ = y;
                return;
            }
            entity.assign<Destination>(x, y);
        }

        void stop(Entity entity, float x, float y) {
            if (!entity)
                return;
            entity.remove<Destination>();
            auto position = entity.component<Position>();
            if (position) {
                // TODO : check for cheating
                position->x_ = x;
                position->y_ = y;
            }
        }
};

class TimeKeeperSystem : public System {
    public:
        virtual ~TimeKeeperSystem() {}

        virtual void update(EntityManager &es, double dt) {
            Component<CharacterInfo> info;
            Component<StatusEffects> effects;
            for (Entity entity : es.entities_with_components(info)) {
                info->dt_ += dt;
                (void)entity;
            }
            for (Entity entity : es.entities_with_components(effects)) {
                for (auto &it : effects->effects_)
                    it.dt_ += dt;
                (void)entity;
            }
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
        T* get() {
            for (auto &it : systems_) {
                T *tmp;
                if ((tmp = dynamic_cast<T*>(it.get())))
                    return tmp;
            }
            return nullptr;
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

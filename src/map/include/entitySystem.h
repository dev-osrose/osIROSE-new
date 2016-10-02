
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

        void calculateSpeed(Entity entity) {
            auto advanced = entity.component<AdvancedInfo>();
            advanced->runSpeed_ = 425;
            if (advanced->moveMode_ == AdvancedInfo::WALK)
                advanced->runSpeed_ = 200;
            if (advanced->moveMode_ == AdvancedInfo::RUN) {
                auto equipped = entity.component<EquippedItems>();
                auto stats = entity.component<Stats>();
                advanced->runSpeed_ += stats->dex_ * .8500001;
                if (equipped->items_[EquippedItems::BOOTS].wearable_.id_ && equipped->items_[EquippedItems::BOOTS].wearable_.life_) {
                    uint16_t realSpeed = equipped->items_[EquippedItems::BOOTS].runSpeed_ - 65;
                    uint16_t realMod = realSpeed / 5;
                    uint16_t dexMod = realMod * (stats->dex_ + realMod) / 23.222;
                    advanced->runSpeed_ += (realSpeed * 5) + realMod + dexMod;
                }
                // TODO : cloth stats
                // TODO : buffs
                // TODO : fairy
                // TODO : dash_up/down
            } else {
                // TODO : ride speed
            }
        }

        void calculateAtkSpeed(Entity entity) {
            (void)entity;
        }

    private:
        EntityManager entityManager_;
        SystemManager systemManager_;
};

#endif /* !_ENTITYSYSTEM_H_ */

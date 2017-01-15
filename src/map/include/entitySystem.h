
#ifndef _ENTITYSYSTEM_H_
# define _ENTITYSYSTEM_H_

#include <functional>
#include <vector>
#include <type_traits>
#include "crosepacket.h"
#include "systems/system.h"
#include <unordered_map>
#include <mutex>
#include <queue>

// FIXME : set those values in the config file/database ?
#define NEARBY_DIST 10000 // in game units, how far is considered 'near' // FIXME : make it entity dependent?
#define POSITION_CHEATING 1000*1000

class EntitySystem {
    public:
        EntitySystem();

        void update(double dt);

        void destroy(Entity entity);

        Entity create();

        template <typename T>
        T& get() {
            return *systemManager_.get<T>();
        }

        bool dispatch(Entity entity, std::unique_ptr<RoseCommon::CRosePacket> packet);

        Entity loadCharacter(uint32_t charId, bool platinium, uint32_t id);
        void saveCharacter(uint32_t  charId, Entity entity);

        static bool isNearby(Entity a, Entity b);

        template <typename ...T>
        void processEntities(std::function<bool(Entity)>&& func) {
            std::lock_guard<std::mutex> lock(access_);
            for (Entity entity : entityManager_.entities_with_components<T...>())
                if (!func(entity))
                    return;
        }

        void registerEntity(Entity entity);

        Entity getEntity(const std::string &name);
        Entity getEntity(uint32_t charId);

        EntityManager &getEntityManager();

    private:
        EntityManager entityManager_;
        SystemManager systemManager_;
        mutable std::mutex access_;
        std::vector<Entity> toDestroy_;
        std::unordered_map<std::string, Entity> nameToEntity_;
        std::unordered_map<uint32_t, Entity> idToEntity_;
        std::queue<std::pair<Entity, std::unique_ptr<RoseCommon::CRosePacket>>> toDispatch_;
};

#endif /* !_ENTITYSYSTEM_H_ */

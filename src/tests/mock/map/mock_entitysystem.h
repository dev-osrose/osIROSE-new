#pragma once

#include "gmock/gmock.h"

using ::testing::_;
using ::testing::Invoke;

#include "entitysystem.h"

// TODO : mock get, isNearby, processEntities
class EntitySystem_Mock : public EntitySystem {
    public:
        MOCK_METHOD1(update, void(double));
        MOCK_METHOD1(destroy, void(Entity));
        MOCK_METHOD0(create, Entity(void));
        bool dispatch(Entity e, std::unique_ptr<RoseCommon::CRosePacket> p) {
            return dispatchProxy(e, p.get());
        }
        MOCK_METHOD2(dispatchProxy, bool(Entity, RoseCommon::CRosePacket*));
        MOCK_METHOD3(loadCharacter, Entity(uint32_t, bool, uint32_t));
        MOCK_METHOD2(saveCharacter, void(uint32_t, Entity));
        MOCK_METHOD1(registerEntity, void(Entity));
        MOCK_METHOD1(getEntity, Entity(const std::string&));
        MOCK_METHOD1(getEntity, Entity(uint32_t));
        MOCK_METHOD0(getEntityManager, EntityManager&(void));
};

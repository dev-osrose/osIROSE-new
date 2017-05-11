#pragma once

#include "gmock/gmock.h"

using ::testing::_;
using ::testing::Invoke;

#include "systemmanager.h"
#include "systems/chatsystem.h"

// TODO: mock add, remove, get
class SystemManager_Mock : public SystemManager {
    public:
        SystemManager_Mock(EntitySystem &a) : SystemManager(a) {}
        MOCK_METHOD1(update, void(double));
        MOCK_METHOD1(wouldDispatch, bool(const RoseCommon::CRosePacket&));
        MOCK_METHOD2(dispatch, bool(Entity, const RoseCommon::CRosePacket&));
        MOCK_METHOD1(getEntity, Entity(const std::string&));
        MOCK_METHOD1(getEntity, Entity(uint32_t));

};

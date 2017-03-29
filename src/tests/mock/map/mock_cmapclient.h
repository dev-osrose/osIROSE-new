#pragma once

#include "gmock/gmock.h"

using ::testing::_;
using ::testing::Invoke;

#include "cmapclient.h"

class CMapClient_Mock : public CMapClient {
    public:
        MOCK_CONST_METHOD1(IsNearby, bool(const RoseCommon::CRoseClient*));
        MOCK_METHOD0(canBeDeleted, void(void));
        MOCK_METHOD1(Send, bool(RoseCommon::CRosePacket&));
};

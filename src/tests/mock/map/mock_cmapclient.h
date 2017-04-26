#pragma once

#include "gmock/gmock.h"

using ::testing::_;
using ::testing::Invoke;

#include "cmapclient.h"

class CMapClient_Mock : public CMapClient {
    public:
      CMapClient_Mock() : CMapClient() {};
      CMapClient_Mock(Core::INetwork* _sock) : CMapClient() {
        set_socket(_sock);
      };


      MOCK_CONST_METHOD1(is_nearby, bool(const RoseCommon::CRoseClient*));
      MOCK_METHOD0(canBeDeleted, void(void));
      MOCK_METHOD1(send, bool(RoseCommon::CRosePacket&));
};

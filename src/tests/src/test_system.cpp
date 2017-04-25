#include "gtest/gtest.h"

#include "systems/chatsystem.h"
#include "mock/map/mock_systemmanager.h"
#include "mock/map/mock_entitysystem.h"
#include "mock/map/mock_cmapclient.h"

using namespace RoseCommon;
using namespace Systems;

using ::testing::Return;

TEST(TestSystems, TestChatSystemSendMsg) {
    EntitySystem_Mock entitySystem;
    SystemManager_Mock mock(entitySystem);
    ChatSystem chat(mock);
    EntityManager man;
    CMapClient_Mock cli;
    Entity e = man.create();
    e.assign<SocketConnector>((CMapClient*)&cli);
    EXPECT_CALL(cli, Send(_)).WillOnce(Return(true));
    chat.sendMsg(e, "test");
    e.remove<SocketConnector>();
    chat.sendMsg(e, "test");
    e.destroy();
    chat.sendMsg(e, "test");
}

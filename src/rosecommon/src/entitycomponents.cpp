#include "entityComponents.h"
#include <assert.h>

uint16_t getId(Entity entity) {
    return entity.component<BasicInfo>()->id_;
}

// TODO: create a powerful error handling system to be able to report problems
std::string &getName(Entity entity) {
    assert(entity.component<BasicInfo>() && "There is no basic info for this entity!");
    return entity.component<BasicInfo>()->name_;
}

CMapClient *getClient(Entity entity) {
    if (!entity.component<SocketConnector>())
        return nullptr;
    return entity.component<SocketConnector>()->client_;
}

bool isConnected(Entity entity) {
    if (!entity.component<BasicInfo>())
        return false;
    return entity.component<BasicInfo>()->loggedIn_.load();
}

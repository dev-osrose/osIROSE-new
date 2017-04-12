#include "entitycomponents.h"
#include "throwassert.h"

uint16_t getId(Entity entity) {
    throw_assert(entity.component<BasicInfo>(), "There is no basic info for entity " << entity.id().id());
    return entity.component<BasicInfo>()->id_;
}

std::string &getName(Entity entity) {
    throw_assert(entity.component<BasicInfo>(), "There is no basic info for entity " << entity.id().id());
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

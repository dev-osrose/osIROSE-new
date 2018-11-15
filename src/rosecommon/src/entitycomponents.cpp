#include "entitycomponents.h"
#include "throwassert.h"
#include <memory>

uint16_t getId(Entity entity) {
    throw_assert(entity.component<BasicInfo>(), "There is no basic info for entity " << entity.id().id());
    return entity.component<BasicInfo>()->id_;
}

std::string &getName(Entity entity) {
    throw_assert(entity.component<BasicInfo>(), "There is no basic info for entity " << entity.id().id());
    return entity.component<BasicInfo>()->name_;
}

std::shared_ptr<CMapClient> getClient(Entity entity) {
    if (!entity.component<SocketConnector>())
        return {};
    return entity.component<SocketConnector>()->client_.lock();
}

bool isOnMap(Entity entity) {
    if (!entity)
        return false;
    if (!entity.component<BasicInfo>())
        return false;
    return entity.component<BasicInfo>()->isOnMap_.load();
}

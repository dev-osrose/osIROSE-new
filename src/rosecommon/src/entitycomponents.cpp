#include "entityComponents.h"
#include <cassert>

uint16_t getId(Entity entity) {
    return entity.component<BasicInfo>()->id_;
}

std::string &getName(Entity entity) {
    return entity.component<BasicInfo>()->name_;
}

CMapClient* getClient(Entity entity) {
    assert(entity.component<SocketConnector>() && entity.component<SocketConnector>()->client_);
    return entity.component<SocketConnector>()->client_;
}


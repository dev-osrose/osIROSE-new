#include "entitySystem.h"
#include "movementsystem.h"
#include "updatesystem.h"

EntitySystem::EntitySystem() : systemManager_(entityManager_) {
    // TODO : use on_component_removed for Destination
    systemManager_.add<Systems::MovementSystem>();
    systemManager_.add<Systems::UpdateSystem>();
}

void EntitySystem::update(double dt) {
    systemManager_.update(dt);
    for (auto it : toDestroy_)
        if (it)
            it.destroy();
    toDestroy_.clear();
}

void EntitySystem::destroy(Entity entity) {
    if (!entity)
        return;
    toDestroy_.push_back(entity);
}

Entity EntitySystem::create() {
    return entityManager_.create();
}

bool EntitySystem::dispatch(Entity entity, const RoseCommon::CRosePacket &packet) {
    if (!entity)
        return false;
    return systemManager_.dispatch(entity, packet);
}

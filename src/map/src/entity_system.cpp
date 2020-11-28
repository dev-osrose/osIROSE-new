#include "entity_system.h"

#include <algorithm>
#include <cmath>
#include <set>

EntitySystem::EntitySystem() {
    logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
}

Entity EntitySystem::create_entity() {
    return registry.create();
}

bool EntitySystem::is_valid(Entity entity) const {
    return registry.valid(entity);
}

void EntitySystem::delete_entity(Entity entity) {
    entitySystem.logger->debug("Deleting entity {}", entity);
    if (entity == entt::null || !registry.valid(entity)) {
        return;
    }
    registry.destroy(entity);
}

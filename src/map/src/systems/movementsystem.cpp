#include "systems/movementsystem.h"
#include <cmath>

using namespace Systems;

MovementSystem::MovementSystem(SystemManager &manager) {
    manager.registerDispatcher(RoseCommon::ePacketType::PAKCS_MOUSE_CMD, &MovementSystem::processMove);
}

void MovementSystem::update(EntityManager &es, double dt) {
    Component<Position> position;
    Component<Destination> destination;
    Component<AdvancedInfo> advanced;
    for (Entity entity : es.entities_with_components(position, destination, advanced)) {
        float dx = destination->x_ - position->x_;
        float dy = destination->y_ - position->y_;
        float distance = std::sqrt(dx * dx + dy * dy);
        float speed = advanced->runSpeed_;
        float ntime = distance / speed;
        if (ntime <= dt || distance == 0) {
            position->x_ = destination->x_;
            position->y_ = destination->y_;
            entity.remove<Destination>();
        } else {
            position->x_ += dx * dt / ntime;
            position->y_ += dy * dt / ntime;
        }
    }
}

void MovementSystem::move(Entity entity, float x, float y) {
    if (!entity)
        return;
    auto dest = entity.component<Destination>();
    if (dest) {
        dest->x_ = x;
        dest->y_ = y;
        return;
    }
    entity.assign<Destination>(x, y);
}

void MovementSystem::stop(Entity entity, float x, float y) {
    if (!entity)
        return;
    entity.remove<Destination>();
    auto position = entity.component<Position>();
    if (position) {
        // TODO : check for cheating
        position->x_ = x;
        position->y_ = y;
    }
}

void MovementSystem::processMove(Entity entity, const RoseCommon::CliMouseCmd &packet) {
    (void)entity;
    (void)packet;
}

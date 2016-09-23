#include "entitySystem.h"

namespace EntitySystemManipulators {

void move(std::shared_ptr<EntitySystem> es, Entity e, float x, float y) {
    es->get<MovementSystem>().move(e, x, y);
}
}

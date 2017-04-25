#include "systems/timesystem.h"

using namespace Systems;

void TimeSystem::update(EntityManager &es, double dt) {
    for (Entity entity : es.entities_with_components<CharacterInfo>()) {
        entity.component<CharacterInfo>()->dt_ += dt;
    }
    for (Entity entity : es.entities_with_components<StatusEffects>()) {
        StatusEffects *effects = entity.component<StatusEffects>();
        for (auto &it : effects->effects_)
            it.dt_ += dt;
    }
}

#include "systems/timesystem.h"

using namespace Systems;


void TimeSystem::update(EntityManager &es, double dt) {
    Component<CharacterInfo> info;
    Component<StatusEffects> effects;
    for (Entity entity : es.entities_with_components(info)) {
        info->dt_ += dt;
        (void)entity;
    }
    for (Entity entity : es.entities_with_components(effects)) {
        for (auto &it : effects->effects_)
            it.dt_ += dt;
        (void)entity;
    }
}

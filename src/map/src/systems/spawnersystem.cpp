#include "systems/spawnersystem.h"

using namespace Systems;
using namespace RoseCommon;

SpawnerSystem::SpawnerSystem(SystemManager &manager) : System(manager) {}

void SpawnerSystem::update(EntityManager &, double dt) {
    for (Entity spawner : es.entities_with_components<Position, Spawner>()) {
        auto spawn = spawner.component<Spawner>();
        spawn->until_update_ += dt;
        if (spawn->until_update_ > spawn->interval_) {
            spawn->until_update_ = 0s;
            if (spawn->current_total_ < spawn->total_on_map_) {
                manager_.buildMobEntity(spawner);
                ++spawn->current_total_;
            }
        }
    }
}

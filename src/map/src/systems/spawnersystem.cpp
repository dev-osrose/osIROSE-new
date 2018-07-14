#include "systems/spawnersystem.h"
#include "cmapserver.h"

using namespace Systems;
using namespace RoseCommon;

SpawnerSystem::SpawnerSystem(SystemManager &manager) : System(manager) {}

void SpawnerSystem::update(EntityManager &es, std::chrono::milliseconds dt) {
    for (Entity spawner : es.entities_with_components<Position, Spawner>()) {
        auto spawn = spawner.component<Spawner>();
        spawn->until_update_ += dt;
        if (spawn->until_update_ > spawn->interval_) {
            spawn->until_update_ = 0ms;
            if (spawn->current_total_ < spawn->total_on_map_) {
                Entity mob = manager_.buildMob(spawner);
                manager_.send(spawner, CMapServer::eSendType::NEARBY, makePacket<ePacketType::PAKWC_MOB_CHAR>(mob));
                ++spawn->current_total_;
            }
        }
    }
}

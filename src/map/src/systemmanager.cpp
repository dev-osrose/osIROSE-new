#include "systemmanager.h"
#include "entitysystem.h"
#include "systems/system.h"

void SystemManager::update(std::chrono::milliseconds dt) {
  auto &entityManager = entitySystem_.getEntityManager();
  for (auto &it : systems_) it.second->update(entityManager, dt);
}

Entity SystemManager::getEntity(const std::string &name) const { return entitySystem_.getEntity(name); }

Entity SystemManager::getEntity(uint32_t charId) const { return entitySystem_.getEntity(charId); }

EntityManager &SystemManager::getEntityManager() { return entitySystem_.getEntityManager(); }

Entity SystemManager::buildItem(Entity creator, RoseCommon::Item &&item) {
  return entitySystem_.buildItemEntity(creator, std::move(item));
}

Entity SystemManager::buildMob(Entity spawner) {
  return entitySystem_.buildMobEntity(spawner);
}

void SystemManager::send(Entity sender, RoseCommon::CRoseServer::eSendType type, RoseCommon::CRosePacket&& _buffer) {
    entitySystem_.send(sender, type, std::move(_buffer));
}

void SystemManager::send(Entity sender, RoseCommon::CRoseServer::eSendType type, const RoseCommon::CRosePacket& _buffer) {
    entitySystem_.send(sender, type, _buffer);
}

void SystemManager::saveCharacter(Entity e) {
    if (auto character = e.component<CharacterInfo>())
        entitySystem_.saveCharacter(character->charId_, e);
}

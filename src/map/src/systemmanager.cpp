#include "systemmanager.h"
#include "systems/system.h"
#include "entitySystem.h"

void SystemManager::update(double dt) {
  auto& entityManager = entitySystem_.getEntityManager();
  for ( auto& it : systems_ )
    it.second->update( entityManager, dt );
}

Entity SystemManager::getEntity(const std::string& name) {
  return entitySystem_.getEntity( name );
}

Entity SystemManager::getEntity(uint32_t charId) {
  return entitySystem_.getEntity( charId );
}

EntityManager& SystemManager::getEntityManager() {
  return entitySystem_.getEntityManager();
}

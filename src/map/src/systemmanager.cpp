#include "systemmanager.h"
#include "entitysystem.h"
#include "systems/system.h"

void SystemManager::update(double dt) {
  auto &entityManager = entitySystem_.getEntityManager();
  for (auto &it : systems_) it.second->update(entityManager, dt);
}

Entity SystemManager::getEntity(const std::string &name) const { return entitySystem_.getEntity(name); }

Entity SystemManager::getEntity(uint32_t charId) const { return entitySystem_.getEntity(charId); }

EntityManager &SystemManager::getEntityManager() { return entitySystem_.getEntityManager(); }

Entity SystemManager::buildItem(Entity creator, RoseCommon::Item &&item) {
  return entitySystem_.buildItemEntity(creator, std::move(item));
}

void SystemManager::SendPacket(const std::shared_ptr<CMapClient>& sender, RoseCommon::CRoseServer::eSendType type,
               RoseCommon::CRosePacket& _buffer) {
    entitySystem_.SendPacket(sender, type, _buffer);
}

void SystemManager::SendPacket(const CMapClient& sender, RoseCommon::CRoseServer::eSendType type,
               RoseCommon::CRosePacket& _buffer) {
    entitySystem_.SendPacket(sender, type, _buffer);
}

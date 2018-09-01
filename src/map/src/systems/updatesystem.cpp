#include "systems/updatesystem.h"

using namespace Systems;

void UpdateSystem::update(EntityManager &es, std::chrono::milliseconds) {
  for (Entity entity : es.entities_with_components<AdvancedInfo, Inventory, Stats>()) calculateSpeed(entity);
  for (Entity entity : es.entities_with_components<BasicInfo, AdvancedInfo>()) calculateCommand(entity);
}

void UpdateSystem::calculateSpeed(Entity entity) {
  auto advanced = entity.component<AdvancedInfo>();
  advanced->runSpeed_ = 425;
  if (advanced->moveMode_ == AdvancedInfo::WALK) advanced->runSpeed_ = 200;
  if (advanced->moveMode_ == AdvancedInfo::RUN) {
    auto inventory = entity.component<Inventory>();
    auto stats = entity.component<Stats>();
    advanced->runSpeed_ += stats->dex_ * .8500001;
    if (inventory->items_[Inventory::BOOTS].id_ && inventory->items_[Inventory::BOOTS].life_) {
      // FIXME : realSpeed = boots.runSpeed - 65
      uint16_t realSpeed = -65;
      uint16_t realMod = realSpeed / 5;
      uint16_t dexMod = realMod * (stats->dex_ + realMod) / 23.222;
      advanced->runSpeed_ += (realSpeed * 5) + realMod + dexMod;
    }
    // TODO : cloth stats
    // TODO : buffs
    // TODO : fairy
    // TODO : dash_up/down
  } else {
    // TODO : ride speed
  }
}

void UpdateSystem::calculateAtkSpeed(Entity) {}

void UpdateSystem::calculateCommand(Entity entity) {
  auto basic = entity.component<BasicInfo>();
  auto destination = entity.component<Destination>();
  auto advanced = entity.component<AdvancedInfo>();
  
  if(advanced->hp_ <= 0) basic->command_ = BasicInfo::DIE;
  else if (destination) basic->command_ = BasicInfo::MOVE;
  else basic->command_ = BasicInfo::STOP;
}

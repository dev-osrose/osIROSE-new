#include "updatesystem.h"

using namespace Systems;


void UpdateSystem::update(EntityManager &es, double) {
    for (Entity entity : es.entities_with_components<AdvancedInfo, EquippedItems, Stats>())
        calculateSpeed(entity);
    for (Entity entity : es.entities_with_components<BasicInfo>())
        calculateCommand(entity);
}

void UpdateSystem::calculateSpeed(Entity entity) {
    auto advanced = entity.component<AdvancedInfo>();
    advanced->runSpeed_ = 425;
    if (advanced->moveMode_ == AdvancedInfo::WALK)
        advanced->runSpeed_ = 200;
    if (advanced->moveMode_ == AdvancedInfo::RUN) {
        auto equipped = entity.component<EquippedItems>();
        auto stats = entity.component<Stats>();
        advanced->runSpeed_ += stats->dex_ * .8500001;
        if (equipped->items_[EquippedItems::BOOTS].wearable_.id_ && equipped->items_[EquippedItems::BOOTS].wearable_.life_) {
            uint16_t realSpeed = equipped->items_[EquippedItems::BOOTS].runSpeed_ - 65;
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
    if (destination)
        basic->command_ = BasicInfo::MOVE;
}

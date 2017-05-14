#include "entitysystem.h"
#include "systems/movementsystem.h"
#include "systems/updatesystem.h"
#include "systems/chatsystem.h"
#include "systems/inventorysystem.h"
#include "systems/partysystem.h"
#include "systems/mapsystem.h"
#include "connection.h"
#include "cmapclient.h"

using namespace RoseCommon;
EntitySystem::EntitySystem() : systemManager_(*this) {
    systemManager_.add<Systems::MovementSystem>();
    systemManager_.add<Systems::UpdateSystem>();
    systemManager_.add<Systems::ChatSystem>();
    systemManager_.add<Systems::InventorySystem>();
    systemManager_.add<Systems::PartySystem>();
    systemManager_.add<Systems::MapSystem>();
}

EntityManager &EntitySystem::getEntityManager() {
    return entityManager_;
}

void EntitySystem::registerEntity(Entity entity) {
    if (!entity)
        return;
    auto basic = entity.component<BasicInfo>();
    if (!basic || basic->name_ == "" || !basic->id_)
        return;
    nameToEntity_[basic->name_] = entity;
    idToEntity_[basic->id_] = entity;
}

Entity EntitySystem::getEntity(const std::string &name) {
    return nameToEntity_[name];
}

Entity EntitySystem::getEntity(uint32_t charId) {
    return idToEntity_[charId];
}

void EntitySystem::update(double dt) {
    std::lock_guard<std::mutex> lock(access_);
    while (toDispatch_.size()) {
        auto tmp = std::move(toDispatch_.front());
        systemManager_.dispatch(tmp.first, *tmp.second);
        toDispatch_.pop();
    }
    systemManager_.update(dt);
    for (auto it : toDestroy_) {
        if (it) {
            saveCharacter(it.component<CharacterInfo>()->charId_, it);
            auto basic = it.component<BasicInfo>();
            nameToEntity_.erase(basic->name_);
            idToEntity_.erase(basic->id_);
            if (auto client = getClient(it))
                client->canBeDeleted();
            it.destroy();
        }
    }
    toDestroy_.clear();
}

void EntitySystem::destroy(Entity entity) {
    if (!entity)
        return;
    std::lock_guard<std::mutex> lock(access_);
    toDestroy_.push_back(entity);
}

Entity EntitySystem::create() {
    return entityManager_.create();
}

bool EntitySystem::isNearby(Entity a, Entity b) {
    return true; // FIXME : actually implement the sight calculation instead of the distance
    if (!a || !b)
        return false;
    auto posa = a.component<Position>();
    auto posb = b.component<Position>();
    if (!posa || !posb)
        return false; // FIXME : is it a bug if there is no position?
    if (posa->map_ != posb->map_)
        return false;
    double dist = (posa->x_ - posb->x_) * (posa->x_ - posb->x_) + (posa->y_ - posb->y_) * (posa->y_ - posb->y_);
    if (dist > NEARBY_DIST)
        return false;
    return true;
}

bool EntitySystem::dispatch(Entity entity, std::unique_ptr<RoseCommon::CRosePacket> packet) {
    if (!entity)
        return false;
    if (systemManager_.wouldDispatch(*packet)) {
        std::lock_guard<std::mutex> lock(access_);
        toDispatch_.emplace(std::make_pair(entity, std::move(packet)));
        return true;
    }
    return false;
}

Entity EntitySystem::loadCharacter(uint32_t charId, bool platinium, uint32_t id) {
    auto conn = Core::connectionPool.getConnection(Core::osirose);
    Core::CharacterTable characters;
    Core::InventoryTable inventoryTable;
    Core::SkillTable skillsTable;

    auto charRes = conn(sqlpp::select(sqlpp::count(characters.id), sqlpp::all_of(characters))
                .from(characters)
                .where(characters.id == charId));

    std::lock_guard<std::mutex> lock(access_);
    auto entity = create();
    if (charRes.front().count != 1L) {
        entity.destroy();
        return Entity();
    }
    const auto &charRow = charRes.front();

    entity.assign<Position>(charRow);
    entity.assign<BasicInfo>(charRow, id);
    entity.assign<Stats>(charRow);
    entity.assign<AdvancedInfo>(charRow);
    entity.assign<CharacterGraphics>(charRow);
    entity.assign<CharacterInfo>(charRow, platinium, charId);

    // TODO : write the pat initialization code
    auto skills = entity.assign<Skills>();
    auto skillRes = conn(sqlpp::select(skillsTable.id, skillsTable.level)
            .from(skillsTable)
            .where(skillsTable.charId == charId));
    skills->loadFromResult(skillRes);

    // TODO : write the hotbar table and loading code
    entity.assign<Hotbar>();
    entity.assign<StatusEffects>();
    entity.assign<RidingItems>();
    entity.assign<BulletItems>();

    // TODO : write the inventory code
    auto inventory = entity.assign<Inventory>();

    auto invRes = conn(sqlpp::select(sqlpp::all_of(inventoryTable))
                .from(inventoryTable)
                .where(inventoryTable.charId == charId));
    inventory->loadFromResult(invRes);

    Systems::UpdateSystem::calculateSpeed(entity);

    registerEntity(entity);
    return entity;
}

void EntitySystem::saveCharacter(uint32_t charId, Entity entity) {
    if (!entity)
        return;
    auto chat = systemManager_.get<Systems::ChatSystem>();
    chat->sendMsg(entity, "Character saved");
    auto conn = Core::connectionPool.getConnection(Core::osirose);
    Core::CharacterTable characters;

    using sqlpp::parameter;
    
    auto update = sqlpp::dynamic_update(conn.get(), characters).dynamic_set().where(characters.id == charId);
    entity.component<Position>()->commitToUpdate(update);
    //entity.component<BasicInfo>()->commitToUpdate(update);
    //entity.component<Stats>()->commitToUpdate(update);
    //entity.component<AdvancedInfo>()->commitToUpdate(update);
    //entity.component<CharacterGraphics>()->commitToUpdate(update);
    //entity.component<CharacterInfo>()->commitToUpdate(update);
    //entity.component<Hotbar>()->commitToUpdate(update);
    //entity.component<StatusEffects>()->commitToUpdate(update);
    //entity.component<RidingItems>()->commitToUpdate(update);
    //entity.component<BulletItems>()->commitToUpdate(update);

    conn->run(update);

    //entity.component<Skills>()->commitToUpdate(updateSkills);
    
    //entity.component<Inventory>()->commitToUpdate(updateInventory);
}

#include <set>
#include <vector>

#include "entitysystem.h"
#include "cmapclient.h"
#include "cmapserver.h"
#include "connection.h"
#include "systems/chatsystem.h"
#include "systems/inventorysystem.h"
#include "systems/luasystem.h"
#include "systems/mapsystem.h"
#include "systems/movementsystem.h"
#include "systems/partysystem.h"
#include "systems/updatesystem.h"
#include "systems/spawnersystem.h"

#include "srv_npcchar.h"

using namespace RoseCommon;
EntitySystem::EntitySystem(CMapServer *server) : systemManager_(*this), server_(server) {
  systemManager_.add<Systems::MovementSystem>();
  systemManager_.add<Systems::UpdateSystem>();
  systemManager_.add<Systems::ChatSystem>();
  systemManager_.add<Systems::InventorySystem>();
  systemManager_.add<Systems::PartySystem>();
  systemManager_.add<Systems::MapSystem>();
  systemManager_.add<Systems::LuaSystem>();
  systemManager_.add<Systems::SpawnerSystem>();
}

EntityManager& EntitySystem::getEntityManager() { return entityManager_; }

Entity EntitySystem::buildItemEntity(Entity creator, RoseCommon::Item&& item) {
  Entity e = create();
  e.assign<Item>(std::move(item));
  auto pos = creator.component<Position>();
  e.assign<Position>(pos->x_, pos->y_, pos->map_, 0);
  auto basic = e.assign<BasicInfo>(id_manager_.get_free_id());
  basic->ownerId_ = creator.component<BasicInfo>()->id_;
  registerEntity(e);
  return e;
}

Entity EntitySystem::buildMobEntity(Entity spawner) {
    Entity e = create();
    auto basic = e.assign<BasicInfo>(id_manager_.get_free_id());
    basic->ownerId_ = spawner.component<BasicInfo>()->id_;
    e.assign<AdvancedInfo>();
    e.assign<CharacterInfo>();
    auto spos = spawner.component<Position>();
    auto pos = e.assign<Position>(spos->x_, spos->y_, spos->map_, 0);
    pos->z_ = spos->z_;
    auto spawn = spawner.component<Spawner>();
    e.assign<Npc>(spawn->mob_id_, 0);
    registerEntity(e);
    return e;
}

void EntitySystem::registerEntity(Entity entity) {
  if (!entity) return;
  if (auto basic = entity.component<BasicInfo>(); basic) {
      if (basic->id_)
          idToEntity_[basic->id_] = entity;
      if (basic->name_.size())
          nameToEntity_[basic->name_] = entity;
  }
}

void EntitySystem::unregisterEntity(Entity entity) {
    if (!entity) return;
    if (auto basic = entity.component<BasicInfo>(); basic) {
        if (basic->id_)
            idToEntity_.erase(basic->id_);
        if (basic->name_.size())
            nameToEntity_.erase(basic->name_);
    }
}

Entity EntitySystem::getEntity(const std::string& name) { return nameToEntity_[name]; }

Entity EntitySystem::getEntity(uint32_t id) { return idToEntity_[id]; }

void EntitySystem::update(std::chrono::milliseconds dt) {
  std::lock_guard<std::mutex> lock(access_);
  for (auto& it : create_commands_) {
    it->execute(*this);
  }
  create_commands_.clear();
  while (toDispatch_.size()) {
    auto tmp = std::move(toDispatch_.front());
    systemManager_.dispatch(tmp.first, *tmp.second);
    toDispatch_.pop();
  }
  systemManager_.update(dt);
  for (auto& it : delete_commands_) {
    it->execute(*this);
  }
  delete_commands_.clear();
}

void EntitySystem::destroy(Entity entity, bool save) {
  if (!entity) return;
  std::lock_guard<std::mutex> lock(access_);
  if (!isOnMap(entity)) return;
  entity.component<BasicInfo>()->isOnMap_.store(false);
  std::unique_ptr<CommandBase> ptr{new Command([save, entity] (EntitySystem &es) mutable {
      if (!entity) return;
      if (!entity.component<Warpgate>()) {
          if (auto client = getClient(entity); client)
            es.SendPacket(client, CMapServer::eSendType::EVERYONE_BUT_ME_ON_MAP,
                               *makePacket<ePacketType::PAKWC_REMOVE_OBJECT>(entity));
          else
            es.SendPacket(std::shared_ptr<CMapClient>{}, CMapServer::eSendType::EVERYONE,
                           *makePacket<ePacketType::PAKWC_REMOVE_OBJECT>(entity));
          if (!entity.component<Npc>()) {
              if (save) es.saveCharacter(entity.component<CharacterInfo>()->charId_, entity);
              auto basic = entity.component<BasicInfo>();
              es.id_manager_.release_id(basic->id_);
          } else if (auto basic = entity.component<BasicInfo>(); basic && basic->ownerId_) {
            // this is a mob, we need to update the spawner
            Entity spawner = es.idToEntity_[basic->ownerId_];
            if (spawner) {
                --spawner.component<Spawner>()->current_total_;
            }
          }
      }
      es.unregisterEntity(entity);
      entity.destroy();
    })};
  delete_commands_.emplace_back(std::move(ptr));
}

Entity EntitySystem::create() { return entityManager_.create(); }

bool EntitySystem::isNearby(Entity a, Entity b) {
    return systemManager_.get<Systems::MovementSystem>()->is_nearby(a, b);
}

bool EntitySystem::dispatch(Entity entity, std::unique_ptr<RoseCommon::CRosePacket> packet) {
  if (!entity) return false;
  if (systemManager_.wouldDispatch(*packet)) {
    std::lock_guard<std::mutex> lock(access_);
    toDispatch_.emplace(std::make_pair(entity, std::move(packet)));
    return true;
  }
  return false;
}

Entity EntitySystem::loadCharacter(uint32_t charId, bool platinium) {
  auto conn = Core::connectionPool.getConnection(Core::osirose);
  Core::CharacterTable characters{};
  Core::InventoryTable inventoryTable{};
  Core::SkillTable skillsTable{};

  auto charRes = conn(sqlpp::select(sqlpp::count(characters.id), sqlpp::all_of(characters))
                          .from(characters)
                          .where(characters.id == charId));

  std::lock_guard<std::mutex> lock(access_);
  auto entity = create();
  if (static_cast<long>(charRes.front().count) != 1L) {
    entity.destroy();
    return Entity();
  }
  const auto& charRow = charRes.front();

  entity.assign<Position>(charRow);
  entity.assign<BasicInfo>(charRow, id_manager_.get_free_id());
  entity.assign<Stats>(charRow);
  entity.assign<AdvancedInfo>(charRow);
  entity.assign<CharacterGraphics>(charRow);
  entity.assign<CharacterInfo>(charRow, platinium, charId);

  // TODO : write the pat initialization code
  auto skills = entity.assign<Skills>();
  auto skillRes =
      conn(sqlpp::select(skillsTable.id, skillsTable.level).from(skillsTable).where(skillsTable.charId == charId));
  skills->loadFromResult(skillRes);

  // TODO : write the hotbar table and loading code
  entity.assign<Hotbar>();
  entity.assign<StatusEffects>();
  entity.assign<RidingItems>();
  entity.assign<BulletItems>();

  // TODO : write the inventory code
  // auto luaSystem = systemManager_.get<Systems::LuaSystem>();
  auto inventory = entity.assign<Inventory>();

  auto invRes =
      conn(sqlpp::select(sqlpp::all_of(inventoryTable)).from(inventoryTable).where(inventoryTable.charId == charId));
  inventory->loadFromResult(invRes, get<Systems::InventorySystem>());

  Systems::UpdateSystem::calculateSpeed(entity);

  entity.assign<Quests>();

  Core::WishTable wish{};
  auto wishRes = conn(sqlpp::select(sqlpp::all_of(wish)).from(wish).where(wish.charId == charId));
  auto wishlist = entity.assign<Wishlist>();
  wishlist->loadFromResult(wishRes, get<Systems::InventorySystem>());

  // auto lua = entity.assign<EntityAPI>();

  // luaSystem->loadScript(entity, "function onInit()\ndisplay('test')\nend");
  // lua->onInit();

  registerEntity(entity);
  return entity;
}

void EntitySystem::saveCharacter(uint32_t charId, Entity entity) {
  if (!entity) return;
  auto conn = Core::connectionPool.getConnection(Core::osirose);
  Core::CharacterTable characters{};

  using sqlpp::parameter;

  auto update = sqlpp::dynamic_update(conn.get(), characters).dynamic_set().where(characters.id == charId);
  entity.component<Position>()->commitToUpdate<decltype(characters)>(update);
  entity.component<BasicInfo>()->commitToUpdate<decltype(characters)>(update);
  entity.component<Stats>()->commitToUpdate<decltype(characters)>(update);
  entity.component<AdvancedInfo>()->commitToUpdate<decltype(characters)>(update);
  // entity.component<CharacterGraphics>()->commitToUpdate(update);
  entity.component<CharacterInfo>()->commitToUpdate<decltype(characters)>(update);
  // entity.component<Hotbar>()->commitToUpdate(update);
  // entity.component<StatusEffects>()->commitToUpdate(update);
  // entity.component<RidingItems>()->commitToUpdate(update);
  // entity.component<BulletItems>()->commitToUpdate(update);

  conn->run(update);

  // entity.component<Skills>()->commitToUpdate(updateSkills);

  Core::InventoryTable inv{};
  auto invRes = conn(sqlpp::select(sqlpp::all_of(inv)).from(inv).where(inv.charId == charId));

  const auto& items = entity.component<Inventory>()->items_;

  std::vector<size_t> toDelete;
  std::vector<size_t> toUpdate;
  std::set<size_t> modified;
  std::vector<size_t> toInsert;

  for (const auto& row : invRes) {
    if (row.slot >= Inventory::maxItems)
      toDelete.emplace_back(row.slot);  // FIXME: that should never happen
    else if (!items[row.slot])
      toDelete.emplace_back(row.slot);
    else if (items[row.slot] != Item(row))
      toUpdate.emplace_back(row.slot);
    modified.insert(row.slot);
  }
  size_t i = 0;
  for (const auto& item : items) {
    if (item && modified.find(i) == modified.end()) toInsert.emplace_back(i);
    ++i;
  }

  for (auto it : toDelete) conn(sqlpp::remove_from(inv).where(inv.charId == charId and inv.slot == it));
  for (auto it : toUpdate) {
    auto update = sqlpp::dynamic_update(conn.get(), inv).dynamic_set().where(inv.charId == charId and inv.slot == it);
    items[it].commitToUpdate<decltype(inv)>(update);
    conn->run(update);
  }
  for (auto it : toInsert) {
    auto insert = sqlpp::dynamic_insert_into(conn.get(), inv).dynamic_set();
    items[it].commitToInsert<decltype(inv)>(insert);
    insert.insert_list.add(inv.slot = it);
    insert.insert_list.add(inv.charId = charId);
    conn->run(insert);
  }
}

Entity EntitySystem::create_warpgate(std::string alias, int dest_map_id, float dest_x, float dest_y, float dest_z,
                    int map_id, float x, float y, float z, float angle,
                    float x_scale, float y_scale, float z_scale) {
    Entity e = create();
    std::unique_ptr<CommandBase> ptr{new Command([alias, dest_map_id, dest_x, dest_y, dest_z,
                                                  map_id, x, y, z, angle, x_scale, y_scale, z_scale,
                                                  e] (EntitySystem &es) mutable {
      if (!e) return;
      e.assign<BasicInfo>(es.id_manager_.get_free_id());

      auto pos = e.assign<Position>(x * 100, y * 100, map_id, 0);

      pos->z_ = static_cast<uint16_t>(z);
      pos->angle_ = angle;

      auto dest = e.assign<Destination>(dest_x * 100, dest_y * 100, dest_map_id);
      dest->z_ = static_cast<uint16_t>(dest_z);
    })};
    std::lock_guard<std::mutex> lock(access_);
    create_commands_.emplace_back(std::move(ptr));
    return e;
}

Entity EntitySystem::create_npc(std::string npc_lua, int npc_id, int map_id, float x, float y, float z, float angle) {
    Entity e = create();
    std::unique_ptr<CommandBase> ptr{new Command([npc_lua, npc_id, map_id, x, y, z, angle, e] (EntitySystem &es) mutable {
      if (!e) return;
      e.assign<BasicInfo>(es.id_manager_.get_free_id());
      e.assign<AdvancedInfo>();
      e.assign<CharacterInfo>();

      uint16_t dialog_id = 0;
      if (!npc_lua.empty()) {
        dialog_id = std::stoi(npc_lua);
      }
      e.assign<Npc>(npc_id, dialog_id);
      auto pos = e.assign<Position>(x * 100, y * 100, map_id, 0);

      pos->z_ = static_cast<uint16_t>(z);
      pos->angle_ = angle;
      //e.assign<EntityApi>();
      // we send the new NPC to the existing clients
      es.SendPacket(std::shared_ptr<CMapClient>{}, CMapServer::eSendType::EVERYONE,
              *makePacket<ePacketType::PAKWC_NPC_CHAR>(e));
    })};
    std::lock_guard<std::mutex> lock(access_);
    create_commands_.emplace_back(std::move(ptr));
    return e;
}

Entity EntitySystem::create_spawner(std::string alias, int mob_id, int mob_count,
                   int spawner_limit, int spawner_interval, int spawner_range,
                   int map_id, float x, float y, float z) {
    Entity e = create();
    std::unique_ptr<CommandBase> ptr{new Command([mob_id, mob_count, spawner_limit, spawner_interval, spawner_range, map_id, x, y, z, e] (EntitySystem &es) mutable {
        if (!e) return;
        e.assign<BasicInfo>(es.id_manager_.get_free_id());
        auto pos = e.assign<Position>(x, y, map_id, 0);
        pos->z_ = static_cast<uint16_t>(z);
        e.assign<Spawner>(mob_id, mob_count, spawner_limit, std::chrono::seconds(spawner_interval), spawner_range);
    })};
    std::lock_guard<std::mutex> lock(access_);
    create_commands_.emplace_back(std::move(ptr));
    return e;
}

void EntitySystem::bulk_destroy(const std::vector<Entity>& s) {
  std::unique_ptr<CommandBase> ptr{new Command([s] (EntitySystem &es) mutable {
    for (auto entity : s) {
        if (!entity) continue;
        if (!entity.component<Warpgate>())
            es.SendPacket(std::shared_ptr<CMapClient>{}, CMapServer::eSendType::EVERYONE,
                         *makePacket<ePacketType::PAKWC_REMOVE_OBJECT>(entity));
        entity.destroy();
    }
  })};
  std::lock_guard<std::mutex> lock(access_);
  delete_commands_.emplace_back(std::move(ptr));
}

LuaScript::ScriptLoader& EntitySystem::get_script_loader() noexcept {
  return server_->get_script_loader();
}

void EntitySystem::SendPacket(const std::shared_ptr<CMapClient>& sender, CMapServer::eSendType type,
                            CRosePacket& _buffer) {
  server_->SendPacket(sender, type, _buffer);
}

void EntitySystem::SendPacket(const CMapClient& sender, CMapServer::eSendType type, CRosePacket& _buffer) {
  server_->SendPacket(sender, type, _buffer);
}

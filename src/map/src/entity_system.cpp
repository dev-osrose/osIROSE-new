#include "entity_system.h"
#include "connection.h"
#include "cmapclient.h"
#include "enumerate.h"
#include "itemdb.h"
#include <entt.hpp>
#include "components/basic_info.h"
#include "components/client.h"
#include "components/computed_values.h"
#include "components/destination.h"
#include "components/faction.h"
#include "components/character_graphics.h"
#include "components/guild.h"
#include "components/hotbar.h"
#include "components/inventory.h"
#include "components/item.h"
#include "components/level.h"
#include "components/life.h"
#include "components/magic.h"
#include "components/npc.h"
#include "components/owner.h"
#include "components/position.h"
#include "components/skills.h"
#include "components/stamina.h"
#include "components/stats.h"
#include "components/status_effects.h"
#include "components/wishlist.h"

#include "chat/normal_chat.h"
#include "chat/whisper_chat.h"
#include "map/change_map.h"
#include "mouse/mouse_cmd.h"

#include "srv_remove_object.h"

#include <algorithm>

EntitySystem::EntitySystem(std::chrono::milliseconds maxTimePerUpdate) : maxTimePerUpdate(maxTimePerUpdate) {
    logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();

    // register recurrent stoof (like saving every 5min)
    using namespace std::chrono_literals;
    add_recurrent_timer(5min, [](EntitySystem& self) {
        self.logger->trace("Saving characters on map");
        self.registry.view<Component::Client>().each([&self](auto entity, auto &client_ptr) {
            (void)client_ptr;
            self.save_character(entity);
        });
    });
    

    // callback for nearby calculations
    registry.construction<Component::Position>().connect<&Nearby::add_entity>(&nearby);
    registry.destruction<Component::Position>().connect<&Nearby::remove_entity>(&nearby);

    // callback for updating the name_to_entity mapping
    registry.construction<Component::BasicInfo>().connect<&EntitySystem::register_name>(this);
    registry.destruction<Component::BasicInfo>().connect<&EntitySystem::unregister_name>(this);

    // callback to stop the moving entity when removed
    registry.destruction<Component::Destination>().connect<&EntitySystem::stop_moving_entity>(this);

    // callback for removing objects
    registry.destruction<Component::Position>().connect<&EntitySystem::remove_object>(this);

    // dispatcher registration
    register_dispatcher(std::function{Chat::normal_chat});
    register_dispatcher(std::function{Chat::whisper_chat});
    register_dispatcher(std::function{Map::change_map_request});
    register_dispatcher(std::function{Mouse::mouse_cmd});
}

void EntitySystem::stop_moving_entity(RoseCommon::Registry&, RoseCommon::Entity entity) {
    logger->trace("EntitySystem::stop_moving_entity");
    // TODO: check cheat here, maybe force stop other clients later
    auto& pos = get_component<Component::Position>(entity);
    const auto& dest = get_component<Component::Destination>(entity);
    pos.x = dest.x;
    pos.y = dest.y;
    pos.z = dest.z;
}

void EntitySystem::remove_object(RoseCommon::Registry&, RoseCommon::Entity entity) {
    logger->trace("EntitySystem::remove_object");
    if (auto* basicInfo = try_get_component<Component::BasicInfo>(entity); basicInfo->id) {
        send_nearby_except_me(entity, RoseCommon::Packet::SrvRemoveObject::create(basicInfo->id));
        idManager.release_id(basicInfo->id);
        basicInfo->id = 0;
    }    
}

uint16_t EntitySystem::get_world_time() const {
    return 0; //TODO: return a correct time
}

void EntitySystem::register_name(RoseCommon::Registry&, RoseCommon::Entity entity) {
    logger->trace("EntitySystem::register_name");
    const auto& basic = get_component<Component::BasicInfo>(entity);
    if (basic.name.size()) {
        name_to_entity.insert({basic.name, entity});
    }
}

void EntitySystem::unregister_name(RoseCommon::Registry&, RoseCommon::Entity entity) {
    logger->trace("EntitySystem::unregister_name");
    const auto& basic = get_component<Component::BasicInfo>(entity);
    if (basic.name.size()) {
        name_to_entity.erase(basic.name);
    }
}

RoseCommon::Entity EntitySystem::get_entity_from_name(const std::string& name) const {
    auto res = name_to_entity.find(name);
    if (res != name_to_entity.end())
        return res->second;
    return entt::null;
}

void EntitySystem::stop() {
    work_queue.kill();
    registry.construction<Component::Position>().disconnect<&Nearby::add_entity>(&nearby);
    registry.destruction<Component::Position>().disconnect<&Nearby::remove_entity>(&nearby);
    registry.construction<Component::BasicInfo>().disconnect<&EntitySystem::register_name>(this);
    registry.destruction<Component::BasicInfo>().disconnect<&EntitySystem::unregister_name>(this);
    registry.destruction<Component::Destination>().disconnect<&EntitySystem::stop_moving_entity>(this);
}

bool EntitySystem::dispatch_packet(RoseCommon::Entity entity, std::unique_ptr<RoseCommon::CRosePacket>&& packet) {
    if (!packet) {
        return false;
    }
    if (!dispatcher.is_supported(*packet.get())) {
        return false;
    }
    add_task(std::move([this, entity, packet = std::move(packet)](EntitySystem& entitySystem) mutable {
        dispatcher.dispatch(entitySystem, entity, std::move(packet));
    }));
    return true;
}

void EntitySystem::run() {
    for (auto [res, task] = work_queue.pop_front(); res;) {
        {
            std::lock_guard<std::mutex> lock(access);
            std::invoke(std::move(task), *this);
        }
        auto [tmp_res, tmp_task] = work_queue.pop_front();
        res = tmp_res;
        task = std::move(tmp_task);
    }
}

void EntitySystem::send_map(const RoseCommon::CRosePacket& packet) const {
    registry.view<const Component::Client>().each([&packet](auto entity, const auto &client_ptr) {
        (void)entity;
        if (auto client = client_ptr.client.lock()) {
            client->send(packet);
        }
    });
}

void EntitySystem::send_nearby(RoseCommon::Entity entity, const RoseCommon::CRosePacket& packet) const {
    registry.view<const Component::Client>().each([entity, this, &packet](auto other, const auto &client_ptr) {
        if (!nearby.is_nearby(*this, entity, other)) return;
        if (auto client = client_ptr.client.lock()) {
            client->send(packet);
        }
    });
}

void EntitySystem::send_nearby_except_me(RoseCommon::Entity entity, const RoseCommon::CRosePacket& packet) const {
    registry.view<const Component::Client>().each([entity, this, &packet](auto other, const auto &client_ptr) {
        if (other != entity) {
            if (!nearby.is_nearby(*this, entity, other)) return;
            if (auto client = client_ptr.client.lock()) {
                client->send(packet);
            }
        }
    });
}

void EntitySystem::send_to(RoseCommon::Entity entity, const RoseCommon::CRosePacket& packet) const {
    if (const auto client_ptr = registry.try_get<const Component::Client>(entity)) {
        if (auto client = client_ptr->client.lock()) {
            client->send(packet);
        }
    }
}

void EntitySystem::delete_entity(RoseCommon::Entity entity) {
    add_task([entity](EntitySystem& entitySystem) {
        entitySystem.logger->debug("Deleting entity {}", entity);
        auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(entity);
        entitySystem.send_nearby_except_me(entity, RoseCommon::Packet::SrvRemoveObject::create(basicInfo.id));
        entitySystem.idManager.release_id(basicInfo.id);
        basicInfo.id = 0;
        entitySystem.registry.destroy(entity);
    });
}

void EntitySystem::update_position(RoseCommon::Entity entity, float x, float y) {
    if (entity == entt::null) return;
    auto* pos = try_get_component<Component::Position>(entity);
    float old_x = 0, old_y = 0;
    bool is_added = false;
    if (!pos) {
        pos = &add_component<Component::Position>(entity);
        pos->z = 0;
        is_added = true;
    } else {
        old_x = pos->x;
        old_y = pos->y;
    }
    const auto old_nearby = get_nearby(entity);

    pos->x = x;
    pos->y = y;
    nearby.update_position(entity, old_x, old_y, x, y);
    if (is_added) {
        return;
    }
    const auto new_nearby = get_nearby(entity);
    std::vector<RoseCommon::Entity> to_remove;
    std::vector<RoseCommon::Entity> to_add;
    std::set_difference(old_nearby.begin(), old_nearby.end(), new_nearby.begin(), new_nearby.end(), std::back_inserter(to_remove));
    std::set_difference(new_nearby.begin(), new_nearby.end(), old_nearby.begin(), old_nearby.end(), std::back_inserter(to_add));

    const auto& basicInfo = get_component<Component::BasicInfo>(entity);
    for (const auto e : to_remove) {
        send_to(e, RoseCommon::Packet::SrvRemoveObject::create(basicInfo.id));
    }
    for (const auto e : to_add) {
        send_to(e, CMapClient::create_srv_player_char(*this, entity));
    }
}

std::vector<RoseCommon::Entity> EntitySystem::get_nearby(RoseCommon::Entity entity) const {
    return nearby.get_nearby(*this, entity);
}

RoseCommon::Entity EntitySystem::load_character(uint32_t charId, bool platinium, uint32_t sessionId, std::weak_ptr<CMapClient> client) {
    using namespace Component;
    auto conn = Core::connectionPool.getConnection(Core::osirose);
    Core::CharacterTable characters{};
    Core::InventoryTable inventoryTable{};
    Core::SkillTable skillsTable{};

    auto charRes = conn(sqlpp::select(sqlpp::count(characters.id), sqlpp::all_of(characters))
                          .from(characters).where(characters.id == charId));

    if (static_cast<long>(charRes.front().count) != 1L) {
        return entt::null;
    }
    const auto& charRow = charRes.front();

    entt::prototype prototype(registry);

    auto& basicInfo = prototype.set<BasicInfo>();
    basicInfo.name = charRow.name;
    basicInfo.id = idManager.get_free_id();
    basicInfo.tag = sessionId;
    basicInfo.teamId = basicInfo.id;
    basicInfo.job = charRow.job;
    basicInfo.statPoints = charRow.statPoints;
    basicInfo.skillPoints = charRow.skillPoints;
    basicInfo.pkFlag = charRow.pkFlag;
    basicInfo.stone = charRow.stone;
    basicInfo.charId = charId;
    
    auto& component_client = prototype.set<Client>();
    component_client.client = client;

    auto& computedValues = prototype.set<ComputedValues>();
    computedValues.command = RoseCommon::Command::STOP;
    computedValues.moveMode = RoseCommon::MoveMode::WALK;
    computedValues.runSpeed = 0;
    computedValues.atkSpeed = 0;
    computedValues.weightRate = 0;
    computedValues.statusFlag = 0;
    computedValues.subFlag = 0;
    
    auto& faction = prototype.set<Faction>();
    faction.id = charRow.factionid;
    faction.rank = charRow.factionRank;
    faction.fame = charRow.fame;
    faction.factionFame[0] = charRow.factionFame1;
    faction.factionFame[1] = charRow.factionFame2;
    faction.points[0] = charRow.factionPoints1;
    faction.points[1] = charRow.factionPoints2;
    faction.points[2] = charRow.factionPoints3;

    auto& characterGraphics = prototype.set<CharacterGraphics>();
    characterGraphics.face = charRow.face;
    characterGraphics.hair = charRow.hair;
    characterGraphics.race = charRow.race;
    
    auto& guild = prototype.set<Guild>();
    guild.id = charRow.clanid;
    guild.contribution = charRow.clanContribution;
    guild.rank = charRow.clanRank;

    prototype.set<Hotbar>();

    auto invRes =
      conn(sqlpp::select(sqlpp::all_of(inventoryTable)).from(inventoryTable)
	   .where(inventoryTable.charId == charId and inventoryTable.storageType == "inventory" or inventoryTable.storageType == "wishlist"));

    auto& wishlist = prototype.set<Wishlist>();
    auto& inventory = prototype.set<Inventory>();
    inventory.zuly = charRow.zuly;
    for (const auto& row : invRes) {
        const bool is_inventory = row.storageType == "inventory";
        const auto maxItems = is_inventory ? RoseCommon::MAX_ITEMS : RoseCommon::MAX_WISHLIST;
        if (row.slot >= maxItems) {
            continue;
        }        
        Item item;
        item.isCreated = false;
        item.life = 1000;
        item.hasSocket = row.socket;
        item.isAppraised = true;
        item.refine = row.refine;
        item.count = row.amount;
        item.gemOpt = row.gemOpt;
        item.price = row.price;
        auto to_emplace = load_item(row.itemtype, row.itemid, item);
        if (is_inventory) {
            inventory.items[row.slot] = to_emplace;
        } else {
            wishlist.items[row.slot] = to_emplace;
        }
    }

    auto& level = prototype.set<Level>();
    level.xp = charRow.exp;
    level.level = charRow.level;
    level.penaltyXp = charRow.penaltyExp;

    auto& life = prototype.set<Life>();
    life.hp = charRow.maxHp / 3; // you only get 30% of your health when login in
    life.maxHp = charRow.maxHp;

    auto& magic = prototype.set<Magic>();
    magic.mp = charRow.maxMp / 3;
    magic.maxMp = charRow.maxMp;

    auto& pos = prototype.set<Position>();
    pos.x = charRow.x;
    pos.y = charRow.y;
    pos.z = 0;
    pos.spawn = charRow.reviveMap;
	pos.map = charRow.map;

    auto skillRes =
      conn(sqlpp::select(skillsTable.id, skillsTable.level).from(skillsTable).where(skillsTable.charId == charId));
    auto& skills = prototype.set<Skills>();
    for (const auto& [i, row] : Core::enumerate(skillRes)) {
        skills.skills[i].set_id(row.id);
        skills.skills[i].set_level(row.level);
    }

    auto& stamina = prototype.set<Stamina>();
    stamina.stamina = charRow.stamina;

    auto& stats = prototype.set<Stats>();
    stats.str = charRow.str;
    stats.dex = charRow.dex;
    stats.int_ = charRow.int_;
    stats.con = charRow.con;
    stats.charm = charRow.charm;
    stats.sense = charRow.sense;
    stats.bodySize = 100;
    stats.headSize = 100;

    prototype.set<StatusEffects>();

    std::lock_guard<std::mutex> lock(access);
    return prototype();
}

void EntitySystem::save_character(RoseCommon::Entity character) {
    add_task([character](EntitySystem& self) {
        auto conn = Core::connectionPool.getConnection(Core::osirose);
        Core::CharacterTable characters{};
        using sqlpp::parameter;
        using namespace Component;
        
        const auto& basicInfo = self.get_component<BasicInfo>(character);
        const auto& faction = self.get_component<Faction>(character);
        const auto& characterGraphics = self.get_component<CharacterGraphics>(character);
        const auto& guild = self.get_component<Guild>(character);
        // TODO: save hotbar
        // TODO: save inventory
        const auto& level = self.get_component<Level>(character);
        const auto& life = self.get_component<Life>(character);
        const auto& magic = self.get_component<Magic>(character);
        const auto& pos = self.get_component<Position>(character);
        // TODO: save skills
        const auto& stamina = self.get_component<Stamina>(character);
        const auto& stats = self.get_component<Stats>(character);
        // TODO: save wishlist

        conn(sqlpp::update(characters).where(characters.id == basicInfo.charId).set(
            characters.name = basicInfo.name,
            characters.job = basicInfo.job,
            characters.statPoints = basicInfo.statPoints,
            characters.skillPoints = basicInfo.skillPoints,
            characters.pkFlag = basicInfo.pkFlag,
            characters.stone = basicInfo.stone,
            characters.factionid = faction.id,
            characters.factionRank = faction.rank,
            characters.fame = faction.fame,
            characters.factionFame1 = faction.factionFame[0],
            characters.factionFame2 = faction.factionFame[1],
            characters.factionPoints1 = faction.points[0],
            characters.factionPoints2 = faction.points[1],
            characters.factionPoints3 = faction.points[2],
            characters.face = characterGraphics.face,
            characters.hair = characterGraphics.hair,
            characters.race = characterGraphics.race,
            characters.clanid = guild.id,
            characters.clanContribution = guild.contribution,
            characters.clanRank = guild.rank,
            characters.exp = level.xp,
            characters.level = level.level,
            characters.penaltyExp = level.penaltyXp,
            characters.maxHp = life.maxHp,
            characters.maxMp = magic.maxMp,
            characters.x = pos.x,
            characters.y = pos.y,
            characters.reviveMap = pos.spawn,
            characters.map = pos.map,
            characters.stamina = stamina.stamina,
            characters.str = stats.str,
            characters.dex = stats.dex,
            characters.int_ = stats.int_,
            characters.con = stats.con,
            characters.charm = stats.charm,
            characters.sense = stats.sense
        ));
    });
}

RoseCommon::Entity EntitySystem::create_item(uint8_t type, uint16_t id) {
    using namespace Component;
    entt::prototype prototype(registry);
    
    const auto &itemDb = RoseCommon::ItemDatabase::getInstance();
    if (!itemDb.itemExists(type, id)) {
        logger->warn("No item {} {} in db", type, id);
        return entt::null;
    }
    const auto& def = itemDb.getItemDef(type, id);
    
    auto& item = prototype.set<Item>();
    item.isCreated = false;
    item.life = 1000;
    item.durability = 100;
    item.hasSocket = false;
    item.isAppraised = false;
    item.refine = 0;
    item.count = 0;
    item.gemOpt = 0;
    item.price = 0;

    prototype.set<RoseCommon::ItemDef>(def);
	
    std::lock_guard<std::mutex> lock(access);
    return prototype();
}

RoseCommon::Entity EntitySystem::load_item(uint8_t type, uint16_t id, Component::Item item) {
    auto entity = create_item(type, id);
    if (entity == entt::null) {
        return entt::null;
    }
    registry.replace<Component::Item>(entity, item);
    return entity;
}

void EntitySystem::save_item(RoseCommon::Entity item, RoseCommon::Entity owner) const {
    // TODO: should be done as a task???
}

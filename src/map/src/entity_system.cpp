#include "entity_system.h"
#include "cmapclient.h"
#include "enumerate.h"
#include "itemdb.h"
#include <entt.hpp>
#include "components/basic_info.h"
#include "components/client.h"
#include "components/computed_values.h"
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

EntitySystem::EntitySystem(std::chrono::milliseconds maxTimePerUpdate) : maxTimePerUpdate(maxTimePerUpdate), nearby(*this) {
    logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();
    
    // callback for nearby calculations
    registry.construction<Component::Position>().connect<&Nearby::add_entity>(&nearby);
    registry.destruction<Component::Position>().connect<&Nearby::remove_entity>(&nearby);
    // dispatcher registration
    register_dispatcher(std::function{Chat::normal_chat});
}

void EntitySystem::stop() {
    work_queue.kill();
    registry.construction<Component::Position>().disconnect<&Nearby::add_entity>(&nearby);
    registry.destruction<Component::Position>().disconnect<&Nearby::remove_entity>(&nearby);
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

void EntitySystem::send_map(const RoseCommon::CRosePacket& packet) {
    registry.view<Component::Client>().each([](auto entity, auto &client_ptr) {
        (void)entity;
        if (auto client = client_ptr.client.lock()) {
            client->send(packet);
        }
    });
}

void EntitySystem::send_nearby(RoseCommon::Entity entity, const RoseCommon::CRosePacket& packet) {
    registry.view<Component::Client>().each([entity, this](auto other, auto &client_ptr) {
        if (!nearby.is_nearby(entity, other)) return;
        if (auto client = client_ptr.client.lock()) {
            client->send(packet);
        }
    });
}

void EntitySystem::sent_to(RoseCommon::Entity entity, const RoseCommon::CRosePacket& packet) {
    if (auto client_ptr = registry.try_get<Component::Client>(entity)) {
        if (auto client = client_ptr->client.lock()) {
            client->send(packet);
        }
    }
}

void EntitySystem::delete_entity(RoseCommon::Entity entity) {
    add_task([entity](EntitySystem& entitySystem) {
        entitySystem.registry.destroy(entity);
    });
}

void EntitySystem::update_position(RoseCommon::Entity entity, float x, float y) {
    if (entity == entt::null) return;
    const auto* pos = try_get_component<Component::Position>();
    float old_x = 0, old_y = 0;
    if (!pos) {
        pos = &add_component<Component::Position>(entity);
        pos->z = 0;
    } else {
        old_x = pos->x;
        old_y = pos->y;
    }
    pos->x = x;
    pos->y = y;
    nearby.update_position(entity, old_x, old_y, x, y);
}

RoseCommon::Entity EntitySystem::load_character(uint32_t charId, bool platinium, uint32_t sessionId, std::weak_ptr<CMapClient> client) {
    using namespace Component;
    auto conn = Core::connectionPool.getConnection(Core::osirose);
    Core::CharacterTable characters{};
    Core::InventoryTable inventoryTable{};
    Core::SkillTable skillsTable{};
    Core::WishTable wish{};

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
    
    auto& component_client = prototype.set<Client>();
    component_client.client = client;

    auto& computedValues = prototype.set<ComputedValues>();
    computedValues.command = RoseCommon::Command::STOP;
    computedValues.isOnMap.store(false);
    computedValues.moveMode = RoseCommon::MoveMode::WALK;
    computedValues.runSpeed = 0;
    computedValues.atkSpeed = 0;
    computedValues.weightRate = 0;
    
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
      conn(sqlpp::select(sqlpp::all_of(inventoryTable)).from(inventoryTable).where(inventoryTable.charId == charId));
    auto& inventory = prototype.set<Inventory>();
    for (const auto& row : invRes) {
        if (row.slot >= RoseCommon::MAX_ITEMS) {
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
        item.price = 0;
        inventory.items[row.slot] = load_item(row.itemtype, row.itemid, item);
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

    auto wishRes = conn(sqlpp::select(sqlpp::all_of(wish)).from(wish).where(wish.charId == charId));
    auto& wishlist = prototype.set<Wishlist>();
    for (const auto& row : wishRes) {
        if (row.slot >= RoseCommon::MAX_WISHLIST) {
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
        wishlist.items[row.slot] = load_item(row.itemtype, row.itemid, item);
    }

    std::lock_guard<std::mutex> lock(access);
    return prototype();
}

void EntitySystem::save_character(RoseCommon::Entity character) const {
    //TODO: should be done as a task
}

RoseCommon::Entity EntitySystem::create_item(uint8_t type, uint16_t id) {
    using namespace Component;
    entt::prototype prototype(registry);
    
    const auto &itemDb = RoseCommon::ItemDatabase::getInstance();
    if (!itemDb.itemExists(type, id)) {
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

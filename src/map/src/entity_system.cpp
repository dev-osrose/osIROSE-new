#include "entity_system.h"
#include "connection.h"
#include "cmapclient.h"
#include "cmapserver.h"
#include "enumerate.h"
#include "itemdb.h"
#include "config.h"
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
#include "components/lua.h"
#include "components/magic.h"
#include "components/mob.h"
#include "components/npc.h"
#include "components/owner.h"
#include "components/position.h"
#include "components/spawner.h"
#include "components/skills.h"
#include "components/stamina.h"
#include "components/stats.h"
#include "components/status_effects.h"
#include "components/target.h"
#include "components/wishlist.h"
#include "components/warpgate.h"

#include "chat/normal_chat.h"
#include "chat/whisper_chat.h"
#include "chat/shout_chat.h"
#include "map/change_map.h"
#include "mouse/mouse_cmd.h"
#include "items/inventory.h"

#include "random.h"

#include "srv_remove_object.h"
#include "srv_switch_server.h"
#include "srv_teleport_reply.h"


#include <algorithm>
#include <cmath>
#include <set>

void update_command(EntitySystem& entitySystem, RoseCommon::Entity entity) {
    if (!entitySystem.has_component<Component::ComputedValues>(entity)) {
        return;
    }
    using RoseCommon::Command;
    auto& computed = entitySystem.get_component<Component::ComputedValues>(entity);
    if (entitySystem.has_component<Component::Destination>(entity)) {
        computed.command = Command::MOVE;
    } else if (entitySystem.has_component<Component::Target>(entity)) {// && entitySystem.has_component<Component::Damage>(entity)) {
        computed.command = Command::ATTACK;
    } else {
        computed.command = Command::STOP;
    }
}

void destroy_lua(RoseCommon::Registry& registry, RoseCommon::Entity entity) {
    {
        auto* lua = registry.try_get<Component::ItemLua>(entity);
        if (lua) {
            if (const auto tmp = lua->api.lock()) {
                tmp->on_delete();
            }
        }
    }
    {
        auto* lua = registry.try_get<Component::NpcLua>(entity);
        if (lua) {
            if (const auto tmp = lua->api.lock()) {
                tmp->on_delete();
            }
        }
    }
}

void check_for_target(EntitySystem& self, RoseCommon::Entity entity) {
    const auto* target = self.try_get_component<Component::Target>(entity);
    if (target && self.is_valid(target->target) && self.has_component<Component::Item>(target->target)) {
        // it's an item, we are at it's location, pick it up
        Items::pickup_item(self, entity, target->target);
        self.remove_component<Component::Target>(entity);
    } else if (target && !self.is_valid(target->target)) {
        // the target is not valid anymore, removing
        self.remove_component<Component::Target>(entity);
    }
}

EntitySystem::EntitySystem(uint16_t map_id, CMapServer *server, std::chrono::milliseconds maxTimePerUpdate) :
    maxTimePerUpdate(maxTimePerUpdate),
    lua_loader(*this, map_id, Core::Config::getInstance().mapServer().luaScript),
    server(server) {
    logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();

    // load item lua
    const auto &itemDb = RoseCommon::ItemDatabase::getInstance();
    itemDb.store_lua([this](uint8_t type, uint16_t id, const std::string& lua) {
        lua_loader.load_lua_item(type, id, lua);
    });

    // register recurrent stoof (like saving every 5min)
    using namespace std::chrono_literals;
    add_recurrent_timer(5min, [](EntitySystem& self) {
        self.logger->trace("Saving characters on map");
        self.registry.view<Component::Client>().each([&self](auto entity, auto &client_ptr) {
            (void)client_ptr;
            self.save_character(entity);
        });
    });

    add_recurrent_timer(100ms, [](EntitySystem& self) {
        // we can use std::for_each(std::execution::par, view.begin(), view.end()) if we need more speed here
        self.registry.view<Component::Stats, Component::Inventory, Component::ComputedValues>().each([&self](auto, auto& stats, auto& inv, auto& computed) {
            (void)inv;
            computed.runSpeed = 433;
            if (computed.moveMode == RoseCommon::MoveMode::WALK) {
                computed.runSpeed = 200;
            }
            if (computed.moveMode == RoseCommon::MoveMode::RUN) {
                computed.runSpeed += stats.dex * 0.8500001;
            }
        });
        self.registry.view<Component::Position, Component::Destination, Component::ComputedValues>().each([&self](auto entity, auto& pos, auto& dest, auto& values) {
            const auto delta = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(100ms);
            const float speed = values.runSpeed;
            const std::chrono::milliseconds ntime{static_cast<int>(1000.f * dest.dist / speed)};
            const float dx = dest.x - pos.x;
            const float dy = dest.y - pos.y;
            const float distance = std::sqrt(dx * dx + dy * dy);
            dest.dist = distance;
            if (ntime <= delta || distance == 0) {
                self.remove_component<Component::Destination>(entity);
                self.update_position(entity, dest.x, dest.y);
                check_for_target(self, entity);
            } else {
                const auto tmp = delta / ntime;
                self.update_position(entity, pos.x + dx * tmp, pos.y + dy * tmp);
            }
        });
    });

    // callback for removing objects
    registry.destruction<Component::Position>().connect<&EntitySystem::remove_object>(this);

    // callback for nearby calculations
    registry.construction<Component::Position>().connect<&Nearby::add_entity>(&nearby);

    // callback for updating the name_to_entity mapping
    registry.construction<Component::BasicInfo>().connect<&EntitySystem::register_name>(this);
    registry.destruction<Component::BasicInfo>().connect<&EntitySystem::unregister_name>(this);

    // callback for destroying lua
    registry.destruction<Component::ItemLua>().connect<&destroy_lua>();

    // dispatcher registration
    register_dispatcher(std::function{Chat::normal_chat});
    register_dispatcher(std::function{Chat::whisper_chat});
    register_dispatcher(std::function{Chat::whisper_chat_srv});
    register_dispatcher(std::function{Chat::shout_chat});
    register_dispatcher(std::function{Chat::announce_chat_srv});
    register_dispatcher(std::function{Map::change_map_request});
    register_dispatcher(std::function{Map::teleport_request});
    register_dispatcher(std::function{Mouse::mouse_cmd});
    register_dispatcher(std::function{Items::equip_item_packet});
    register_dispatcher(std::function{Items::drop_item_packet});

    // load npc/mob/warpgates/spawn points lua
    lua_loader.load_file(Core::Config::getInstance().mapServer().luaScript);
}

uint16_t EntitySystem::get_free_id() {
    return idManager.get_free_id();
}

void EntitySystem::remove_spawner(RoseCommon::Registry&, RoseCommon::Entity entity) {
    logger->trace("EntitySystem::remove_spawner");
    auto& spawner = get_component<Component::Spawner>(entity);
    spawner.callback.cancel();
}

void EntitySystem::remove_object(RoseCommon::Registry& r, RoseCommon::Entity entity) {
    logger->trace("EntitySystem::remove_object");
    if (auto* basicInfo = try_get_component<Component::BasicInfo>(entity); basicInfo->id) {
        send_nearby_except_me(entity, RoseCommon::Packet::SrvRemoveObject::create(basicInfo->id));
        idManager.release_id(basicInfo->id);
        id_to_entity.erase(basicInfo->id);
        basicInfo->id = 0;
    }
    nearby.remove_entity(r, entity);
}

uint16_t EntitySystem::get_world_time() const {
    return 0; //TODO: return a correct time
}

bool EntitySystem::is_valid(RoseCommon::Entity entity) const {
    return registry.valid(entity);
}

void EntitySystem::register_name(RoseCommon::Registry&, RoseCommon::Entity entity) {
    logger->trace("EntitySystem::register_name");
    const auto& basic = get_component<Component::BasicInfo>(entity);
    if (basic.name.size()) {
        name_to_entity.insert({basic.name, entity});
    }
    if (basic.id) {
        id_to_entity.insert({basic.id, entity});
    }
}

void EntitySystem::unregister_name(RoseCommon::Registry&, RoseCommon::Entity entity) {
    logger->trace("EntitySystem::unregister_name");
    const auto& basic = get_component<Component::BasicInfo>(entity);
    if (basic.name.size()) {
        name_to_entity.erase(basic.name);
    }
    if (basic.id) {
        id_to_entity.erase(basic.id);
    }
}

RoseCommon::Entity EntitySystem::get_entity_from_name(const std::string& name) const {
    auto res = name_to_entity.find(name);
    if (res != name_to_entity.end())
        return res->second;
    return entt::null;
}

RoseCommon::Entity EntitySystem::get_entity_from_id(uint16_t id) const {
    auto res = id_to_entity.find(id);
    if (res != id_to_entity.end())
        return res->second;
    return entt::null;
}

void EntitySystem::stop() {
    work_queue.kill();
    registry.construction<Component::Position>().disconnect<&Nearby::add_entity>(&nearby);
    registry.destruction<Component::Position>().disconnect<&EntitySystem::remove_object>(this);
    registry.construction<Component::BasicInfo>().disconnect<&EntitySystem::register_name>(this);
    registry.destruction<Component::BasicInfo>().disconnect<&EntitySystem::unregister_name>(this);
}

bool EntitySystem::dispatch_packet(RoseCommon::Entity entity, std::unique_ptr<RoseCommon::CRosePacket>&& packet) {
    if (!packet) {
        return false;
    }
    if (!dispatcher.is_supported(*packet.get())) {
        return false;
    }
    add_task([this, entity, packet = std::move(packet)](EntitySystem& entitySystem) mutable {
        dispatcher.dispatch(entitySystem, entity, std::move(packet));
    });
    return true;
}

void EntitySystem::run() {
    for (auto [res, task] = work_queue.pop_front(); res;) {
        {
            std::lock_guard<std::recursive_mutex> lock(access);
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

void EntitySystem::send_to(RoseCommon::Entity entity, const RoseCommon::CRosePacket& packet, bool force) const {
    if (const auto client_ptr = registry.try_get<const Component::Client>(entity)) {
        if (auto client = client_ptr->client.lock()) {
            client->send(packet, force);
        }
    }
}

void EntitySystem::send_to_entity(RoseCommon::Entity entity, RoseCommon::Entity other) const {
    if (try_get_component<Component::Npc>(other)) {
        send_to(entity, CMapClient::create_srv_npc_char(*this, other));
    } else if (try_get_component<Component::Mob>(other)) {
        send_to(entity, CMapClient::create_srv_mob_char(*this, other));
    } else if (try_get_component<Component::Client>(other)) {
        send_to(entity, CMapClient::create_srv_player_char(*this, other));
    } else if (try_get_component<Component::Item>(other)) {
        send_to(entity, CMapClient::create_srv_drop_item(*this, other));
    }
}

void EntitySystem::delete_entity(RoseCommon::Entity entity) {
    add_task([entity](EntitySystem& entitySystem) {
        entitySystem.logger->debug("Deleting entity {}", entity);
        if (entity == entt::null || !entitySystem.registry.valid(entity)) {
            return;
        }
        if (auto* basicInfo = entitySystem.try_get_component<Component::BasicInfo>(entity)) {
            entitySystem.send_nearby_except_me(entity, RoseCommon::Packet::SrvRemoveObject::create(basicInfo->id));
            entitySystem.idManager.release_id(basicInfo->id);
            entitySystem.id_to_entity.erase(basicInfo->id);
            basicInfo->id = 0;
        }
        if (auto* owner = entitySystem.try_get_component<Component::Owner>(entity)) {
            // we have an owner, update it
            if (auto* spawner = entitySystem.try_get_component<Component::Spawner>(owner->owner)) {
                spawner->mobs.erase(std::remove(spawner->mobs.begin(), spawner->mobs.end(), entity), spawner->mobs.end());
            }
        }
        // we delete all entities owned by this entity except if it's an item, then we allow it for everybody
        entitySystem.registry.view<Component::Owner>().each([&entitySystem, entity](auto en, auto& owner) {
            if (owner.owner == entity) {
                if (!entitySystem.has_component<Component::Item>(en)) {
                    entitySystem.delete_entity(en);
                } else {
                    entitySystem.remove_component<Component::Owner>(en);
                    auto& basicInfo = entitySystem.get_component<Component::BasicInfo>(en);
                    basicInfo.teamId = basicInfo.id;
                }
            }
        });
        // we remove the target from others as it is deleted
        entitySystem.registry.view<Component::Target>().each([&entitySystem, entity](auto en, auto& target) {
            if (target.target == entity) {
                entitySystem.remove_component<Component::Target>(en);
            }
        });
        if (auto* inv = entitySystem.try_get_component<Component::Inventory>(entity)) {
            for (auto & item : inv->items) {
                if (item != entt::null) {
                    entitySystem.delete_entity(item);
                }
            }
        }
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

    // check for warpgates if entity can be teleported
    if (has_component<Component::BasicInfo>(entity) && has_component<Component::Client>(entity)) {
        registry.view<Component::Warpgate, Component::Destination>().each([this, pos, entity](auto, auto& warpgate, auto& destination) {
            if (!warpgate.is_point_in(pos->x, pos->y, pos->z)) {
                return;
            }
            float x = destination.x;
            float y = destination.y;
            uint16_t map = warpgate.dest_map;
            add_task([entity, x, y, map](EntitySystem& self) {
                self.teleport_entity(entity, x, y, map);
            });
        });
    }

    const auto new_nearby = get_nearby(entity);
    std::vector<RoseCommon::Entity> to_remove;
    std::vector<RoseCommon::Entity> to_add;
    std::set_difference(old_nearby.begin(), old_nearby.end(), new_nearby.begin(), new_nearby.end(), std::back_inserter(to_remove));
    std::set_difference(new_nearby.begin(), new_nearby.end(), old_nearby.begin(), old_nearby.end(), std::back_inserter(to_add));

    if (!is_added) {
        const auto& basicInfo = get_component<Component::BasicInfo>(entity);
        for (const auto other : to_remove) {
            send_to(other, RoseCommon::Packet::SrvRemoveObject::create(basicInfo.id));
            if (const auto* basic = try_get_component<Component::BasicInfo>(other)) {
                send_to(entity, RoseCommon::Packet::SrvRemoveObject::create(basic->id));
            }
        }
    }
    for (const auto other : to_add) {
        send_to_entity(entity, other);
        send_to_entity(other, entity);
    }
}

void EntitySystem::teleport_entity(RoseCommon::Entity entity, float x, float y, uint16_t map_id) {
    logger->trace("EntitySystem::teleport_entity");
    logger->debug("Teleporting {} to {}-{}-{}", entity, map_id, x, y);
    remove_component<Component::Target>(entity);
    remove_component<Component::Destination>(entity);
    auto& pos = get_component<Component::Position>(entity);
    if (pos.map == map_id) {
        // we teleport the character on the same map
        const auto& basic = get_component<Component::BasicInfo>(entity);
        const auto& computed_values = get_component<Component::ComputedValues>(entity);
        send_to(entity, RoseCommon::Packet::SrvTeleportReply::create(
            basic.id,
            map_id,
            x,
            y,
            computed_values.moveMode,
            0 // computed_values.rideMode (FIXME: we don't have it yet)
        ));
        update_position(entity, x, y);
    } else {
        // we update the position to save it, we remove it first from the nearby list
        nearby.remove_entity(registry, entity);
        pos.x = x;
        pos.y = y;
        pos.map = map_id;
        save_character(entity);
        if (const auto client_ptr = registry.try_get<const Component::Client>(entity)) {
            if (auto client = client_ptr->client.lock()) {
                client->switch_server();
                auto& config = Core::Config::getInstance();
                // force send the packet as the client isn't on the map anymore technically
                send_to(entity, RoseCommon::Packet::SrvSwitchServer::create(
                    config.mapServer().clientPort + map_id,
                    client->get_session_id(),
                    0,
                    config.serverData().externalIp
                ), true);
            }
        }
    }
}

std::vector<RoseCommon::Entity> EntitySystem::get_nearby(RoseCommon::Entity entity) const {
    const auto res = nearby.get_nearby(*this, entity);
    for (auto en : res) {
        update_command(*const_cast<EntitySystem*>(this), en);
    }
    return res;
}

RoseCommon::Entity EntitySystem::load_character(uint32_t charId, uint16_t access_level, uint32_t sessionId, std::weak_ptr<CMapClient> client) {
    using namespace Component;
    auto conn = Core::connectionPool.getConnection<Core::Osirose>();
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
    component_client.access_level = access_level;

    auto& computedValues = prototype.set<ComputedValues>();
    computedValues.command = RoseCommon::Command::STOP;
    computedValues.moveMode = RoseCommon::MoveMode::RUN;
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
        .where(inventoryTable.charId == charId and
           (inventoryTable.storageType == "inventory" or inventoryTable.storageType == "wishlist")));

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
    life.hp = charRow.currentHp <= 0 ? charRow.maxHp / 3 : charRow.currentHp;
    life.maxHp = charRow.maxHp;

    auto& magic = prototype.set<Magic>();
    magic.mp = charRow.currentMp <= 0 ? charRow.maxMp / 3 : charRow.currentMp;
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

    std::lock_guard<std::recursive_mutex> lock(access);
    return prototype();
}

void EntitySystem::save_character(RoseCommon::Entity character) {
    add_task([character](EntitySystem& self) {
        auto conn = Core::connectionPool.getConnection<Core::Osirose>();
        Core::CharacterTable characters{};
        Core::InventoryTable inventory{};
        using sqlpp::parameter;
        using namespace Component;

        const auto& basicInfo = self.get_component<BasicInfo>(character);
        const auto& faction = self.get_component<Faction>(character);
        const auto& characterGraphics = self.get_component<CharacterGraphics>(character);
        const auto& guild = self.get_component<Guild>(character);
        // TODO: save hotbar
        // TODO: save inventory
        const auto& inv = self.get_component<Inventory>(character);
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
            characters.zuly = inv.zuly,
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

        auto invRes = conn(sqlpp::select(sqlpp::all_of(inventory)).from(inventory)
                .where(inventory.charId == basicInfo.charId));
        std::vector<size_t> to_delete;
        std::vector<size_t> to_add;
        std::set<size_t> modified;
        std::vector<size_t> to_update;

        for (const auto& row : invRes) {
            if (row.slot >= RoseCommon::MAX_ITEMS) {
                to_delete.push_back(row.slot);
            } else if (inv.items[row.slot] == entt::null) {
                to_delete.push_back(row.slot);
            } else {
                const auto& item = self.get_component<Component::Item>(inv.items[row.slot]);
                const auto& itemDef = self.get_component<RoseCommon::ItemDef>(inv.items[row.slot]);
                if (itemDef.type != row.itemtype || itemDef.id != row.itemid || item.count != row.amount
                    || item.refine != row.refine || item.gemOpt != row.gemOpt || item.hasSocket != row.socket) {
                    to_update.push_back(row.slot);
                }
            }
            modified.insert(row.slot);
        }

        for (const auto [i, it] : Core::enumerate(inv.items)) {
            if (it != entt::null && modified.find(i) == modified.end()) {
                to_add.push_back(i);
            }
        }

        for (const auto it : to_delete) {
            conn(sqlpp::remove_from(inventory).where(inventory.charId == basicInfo.charId and inventory.slot == it));
        }
        for (const auto it : to_update) {
            const auto& item = self.get_component<Component::Item>(inv.items[it]);
            const auto& itemDef = self.get_component<RoseCommon::ItemDef>(inv.items[it]);
            conn(sqlpp::update(inventory).set(
                inventory.itemid = itemDef.id,
                inventory.itemtype = static_cast<int>(itemDef.type),
                inventory.amount = item.count,
                inventory.refine = item.refine,
                inventory.gemOpt = item.gemOpt,
                inventory.socket = static_cast<int>(item.hasSocket)
            ).where(inventory.charId == basicInfo.charId and inventory.slot == it));
        }
        for (const auto it : to_add) {
            const auto& item = self.get_component<Component::Item>(inv.items[it]);
            const auto& itemDef = self.get_component<RoseCommon::ItemDef>(inv.items[it]);
            conn(sqlpp::insert_into(inventory).set(
                inventory.itemid = itemDef.id,
                inventory.itemtype = static_cast<int>(itemDef.type),
                inventory.amount = item.count,
                inventory.refine = item.refine,
                inventory.gemOpt = item.gemOpt,
                inventory.socket = static_cast<int>(item.hasSocket),
                inventory.slot = it,
                inventory.charId = basicInfo.charId
            ));
        }
    });
}

RoseCommon::Entity EntitySystem::create_item(uint8_t type, uint16_t id, uint32_t count) {
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
    item.count = count;
    item.gemOpt = 0;
    item.price = 0;
    item.is_zuly = false;

    prototype.set<RoseCommon::ItemDef>(def);

    auto& lua = prototype.set<ItemLua>();
    lua.api = lua_loader.get_lua_item(type, id);
    if (const auto tmp = lua.api.lock(); tmp) {
        tmp->on_init();
    }

    std::lock_guard<std::recursive_mutex> lock(access);
    return prototype();
}

RoseCommon::Entity EntitySystem::create_zuly(int64_t zuly) {
    using namespace Component;
    entt::prototype prototype(registry);

    auto& item = prototype.set<Item>();
    item.is_zuly = true;
    item.count = zuly;

    std::lock_guard<std::recursive_mutex> lock(access);
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

void EntitySystem::save_item([[maybe_unused]] RoseCommon::Entity item, [[maybe_unused]] RoseCommon::Entity owner) const {
}

RoseCommon::Entity EntitySystem::create_npc(int quest_id, int npc_id, int map_id, float x, float y, float z, float angle) {
    logger->trace("EntitySystem::create_npc");
    using namespace Component;
    entt::prototype prototype(registry);
    auto& basic = prototype.set<BasicInfo>();
    basic.id = idManager.get_free_id();
    basic.teamId = basic.id;

    auto& computed_values = prototype.set<ComputedValues>();
    computed_values.moveMode = RoseCommon::MoveMode::WALK;
    computed_values.command = RoseCommon::Command::STOP;
    computed_values.statusFlag = 0;

    auto& life = prototype.set<Life>();
    life.hp = 1;
    life.maxHp = 1;

    auto& pos = prototype.set<Position>();
    pos.x = x * 100;
    pos.y = y * 100;
    pos.z = static_cast<uint16_t>(z);
    pos.map = map_id;

    auto& npc = prototype.set<Npc>();
    npc.id = npc_id;
    npc.quest = quest_id;
    npc.angle = angle;
    npc.event_status = 0;

    // TODO: add lua

    return prototype();
}

RoseCommon::Entity EntitySystem::create_warpgate([[maybe_unused]] std::string alias,
    int id, int dest_map_id, float dest_x, float dest_y, float dest_z,
    float min_x, float min_y, float min_z,
    float max_x, float max_y, float max_z) {
    logger->trace("EntitySystem::create_warpgate");
    using namespace Component;
    entt::prototype prototype(registry);

    auto& warpgate = prototype.set<Warpgate>();
    warpgate.id = id;
    warpgate.dest_map = dest_map_id;
    warpgate.min_x = min_x * 100.f;
    warpgate.min_y = min_y * 100.f;
    warpgate.min_z = min_z;
    warpgate.max_x = max_x * 100.f;
    warpgate.max_y = max_y * 100.f;
    warpgate.max_z = max_z;

    auto& dest = prototype.set<Destination>();
    dest.x = dest_x * 100;
    dest.y = dest_y * 100;
    dest.z = dest_z * 100;

    // TODO: add lua

    return prototype();
}

RoseCommon::Entity EntitySystem::create_spawner([[maybe_unused]] std::string alias,
        int mob_id, int mob_count, int limit, int interval, int range, int map_id, float x, float y, float z) {
    logger->trace("EntitySystem::create_spawner");
    using namespace Component;
    entt::prototype prototype(registry);

    auto& spawner = prototype.set<Spawner>();
    spawner.mob_id = mob_id;
    spawner.max_mobs = mob_count;
    spawner.max_once = limit;
    spawner.interval = std::chrono::seconds(interval);
    spawner.range = range * 100;
    spawner.mobs.reserve(mob_count);

    auto& pos = prototype.set<Position>();
    pos.x = x * 100;
    pos.y = y * 100;
    pos.z = z * 100;
    pos.map = map_id;

    auto entity = prototype();

    spawner.callback = add_recurrent_timer(spawner.interval, [entity](EntitySystem& self) {
        auto& spawner = self.get_component<Spawner>(entity);
        if (spawner.mobs.size() < static_cast<size_t>(spawner.max_mobs)) {
            int number = Core::Random::getInstance().get_uniform(0, std::min(static_cast<size_t>(spawner.max_once), spawner.max_mobs - spawner.mobs.size()));
            for (int i = 0; i < number; ++i) {
                const auto mob = self.create_mob(entity);
                spawner.mobs.push_back(mob);
                self.send_nearby_except_me(entity, CMapClient::create_srv_mob_char(self, mob));
            }
        }
    });

    // TODO: add lua

    return entity;
}

RoseCommon::Entity EntitySystem::create_mob(RoseCommon::Entity spawner) {
    logger->trace("EntitySystem::create_mob");
    using namespace Component;
    entt::prototype prototype(registry);
    const auto& spawn = get_component<Spawner>(spawner);
    const auto& spos = get_component<Position>(spawner);

    auto data = lua_loader.get_data(spawn.mob_id);

    auto& basic_info = prototype.set<BasicInfo>();
    basic_info.id = idManager.get_free_id();
    basic_info.tag = basic_info.id;
    basic_info.teamId = -1;

    auto& position = prototype.set<Position>();
    auto pos = Core::Random::getInstance().random_in_circle(spos.x, spos.y, static_cast<float>(spawn.range));
    position.x = std::get<0>(pos);
    position.y = std::get<1>(pos);
    position.z = spos.z;
    position.map = spos.map;

    auto& computed_values = prototype.set<ComputedValues>();
    computed_values.command = RoseCommon::Command::STOP;
    computed_values.moveMode = RoseCommon::MoveMode::WALK;
    computed_values.runSpeed = data ? data.value().get_run_speed() : 0;
    computed_values.atkSpeed = data ? data.value().get_attack_spd() : 0;
    computed_values.weightRate = 0;
    computed_values.statusFlag = 0;
    computed_values.subFlag = 0;

    auto& life = prototype.set<Life>();
    life.hp = data ? data.value().get_hp() : 1;
    life.maxHp = life.hp;

    auto& mob = prototype.set<Mob>();
    mob.id = spawn.mob_id;
    mob.quest = 0;

    auto& owner = prototype.set<Owner>();
    owner.owner = spawner;

    // TODO: add lua

    return prototype();
}

void EntitySystem::send_to_maps(const RoseCommon::CRosePacket& packet, const std::vector<uint16_t>& maps) const {
    server->send_to_maps(packet, maps);
}

void EntitySystem::send_to_chars(const RoseCommon::CRosePacket& packet, const std::vector<std::string>& chars) const {
    server->send_to_chars(packet, chars);
}

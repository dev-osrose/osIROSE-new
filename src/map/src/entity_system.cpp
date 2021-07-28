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
#include "components/combat.h"
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
#include "components/player_spawn.h"
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
#include "combat/combat.h"
#include "combat/player.h"
#include "items/inventory.h"

#include "utils/calculation.h"
#include "utils/transfer_to_char_server.h"
#include "utils/transfer_to_char.h"

#include "random.h"

#include "cli_party_req.h"
#include "srv_party_req.h"
#include "srv_party_reply.h"
#include "cli_party_reply.h"

#include "srv_remove_object.h"
#include "srv_switch_server.h"
#include "srv_teleport_reply.h"
#include "isc_client_status.h"


#include <algorithm>
#include <cmath>
#include <set>

void update_command(EntitySystem& entitySystem, Entity entity) {
    if (!entitySystem.has_component<Component::ComputedValues>(entity)) {
        return;
    }
    using RoseCommon::Command;
    auto& computed = entitySystem.get_component<Component::ComputedValues>(entity);
    if (entitySystem.has_component<Component::Destination>(entity)) {
        computed.command = Command::MOVE;
    } else if (entitySystem.has_component<Component::Target>(entity) && entitySystem.has_component<Component::Combat>(entity)) {
        computed.command = Command::ATTACK;
    } else {
        computed.command = Command::STOP;
    }
}

void destroy_lua(Registry& registry, Entity entity) {
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

void check_for_target(EntitySystem& self, Entity entity) {
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
    loading(true),
    maxTimePerUpdate(maxTimePerUpdate),
    lua_loader(*this, map_id, Core::Config::getInstance().mapServer().luaScript),
    server(server) {
    logger = Core::CLog::GetLogger(Core::log_type::GENERAL).lock();

    // load item lua
    const auto &itemDb = RoseCommon::ItemDatabase::getInstance();
    itemDb.store_lua([this](uint8_t type, uint16_t id, const std::string& lua) {
        lua_loader.load_lua_item(type, id, lua, [](Entity, int, int) {}, [](Entity, int, int) {});
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
        self.registry.view<Component::Stats, Component::Inventory, Component::ComputedValues>().each([&self](auto entity, [[maybe_unused]] auto& stats, [[maybe_unused]] auto& inv, auto& computed) {
            switch(computed.moveMode) {
              case RoseCommon::MoveMode::WALK:
              {
                // This is a fixed speed
                computed.runSpeed = RoseCommon::WALK_SPEED;
                break;
              }
              default:
              {
                computed.runSpeed = Calculations::get_runspeed(self, entity);
                break;
              }
            }
            if(computed.runSpeed < 200) computed.runSpeed = RoseCommon::WALK_SPEED;

            computed.atkSpeed = Calculations::get_attackspeed(self, entity);
            // get original speed + any move speed increase from items (stat 8) - any movement decrease from items (stat 9)
            if(computed.atkSpeed < 30) computed.atkSpeed = 30;
        });
    });

    prevTime = Core::Time::GetTickCount();
    add_recurrent_timer(50ms, [](EntitySystem& self) {
      auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(Core::Time::GetTickCount() - self.prevTime).count();

      self.registry.view<Component::Position, Component::Destination, Component::ComputedValues>().each([&self, &dt](auto entity, auto& pos, auto& dest, auto& values) {
        const std::chrono::duration<double, std::milli> delta{dt};
        const float speed = values.runSpeed;
        const std::chrono::milliseconds ntime{static_cast<int>(1000.f * dest.dist / speed)};
        const float dx = dest.x - pos.x;
        const float dy = dest.y - pos.y;
        const float distance = std::sqrt(dx * dx + dy * dy);
        dest.dist = distance;
        if (ntime <= delta || distance <= 0.0001f) {
          self.remove_component<Component::Destination>(entity);
          self.update_position(entity, dest.x, dest.y);
          check_for_target(self, entity);
        } else {
          const auto tmp = delta / ntime;
          self.update_position(entity, pos.x + dx * tmp, pos.y + dy * tmp);
        }
      });

      // we can use std::for_each(std::execution::par, view.begin(), view.end()) if we need more speed here
      dt = std::chrono::duration_cast<std::chrono::milliseconds>(Core::Time::GetTickCount() - self.prevTime).count();
      self.registry.view<Component::Level, Component::Life, Component::ComputedValues>().each([&self, &dt](auto entity, [[maybe_unused]] auto& level, [[maybe_unused]] auto& life, [[maybe_unused]] auto& values) {
        Combat::update(self, entity, dt);
      });
      self.prevTime = Core::Time::GetTickCount();
    });

    // callback for removing objects
    registry.on_destroy<Component::Position>().connect<&EntitySystem::remove_object>(this);

    // callback for nearby calculations
    registry.on_construct<Component::Position>().connect<&Nearby::add_entity>(&nearby);

    // callback for updating the name_to_entity mapping
    registry.on_construct<Component::BasicInfo>().connect<&EntitySystem::register_name>(this);
    registry.on_destroy<Component::BasicInfo>().connect<&EntitySystem::unregister_name>(this);

    // callback for destroying lua
    registry.on_destroy<Component::ItemLua>().connect<&destroy_lua>();

    // dispatcher registration
    register_dispatcher(std::function{Chat::normal_chat});
    register_dispatcher(std::function{Chat::whisper_chat});
    register_dispatcher(std::function{Chat::whisper_chat_srv});
    register_dispatcher(std::function{Chat::shout_chat});
    register_dispatcher(std::function{Chat::announce_chat_srv});
    register_dispatcher(std::function{Map::change_map_request});
    register_dispatcher(std::function{Map::teleport_request});
    register_dispatcher(std::function{Mouse::mouse_cmd});
    register_dispatcher(std::function{Mouse::stop_moving});
    register_dispatcher(std::function{Combat::attack});
    register_dispatcher(std::function{Combat::hp_request});
    register_dispatcher(std::function{Combat::revive});
    register_dispatcher(std::function{Player::add_stat});
    register_dispatcher(std::function{Player::toggle_player_move});
    register_dispatcher(std::function{Player::set_animation});
    register_dispatcher(std::function{Items::equip_item_packet});
    register_dispatcher(std::function{Items::drop_item_packet});

    register_dispatcher(std::function{Utils::transfer_to_char_server<RoseCommon::Packet::CliPartyReq>});
    register_dispatcher(std::function{Utils::transfer_to_char<RoseCommon::Packet::SrvPartyReq>});
    register_dispatcher(std::function{Utils::transfer_to_char_server<RoseCommon::Packet::CliPartyReply>});
    register_dispatcher(std::function{Utils::transfer_to_char<RoseCommon::Packet::SrvPartyReply>});

    // load npc/mob/warpgates/spawn points lua
    lua_loader.load_file(Core::Config::getInstance().mapServer().luaScript);
    loading = false;
}

uint16_t EntitySystem::get_free_id() {
    return idManager.get_free_id();
}

void EntitySystem::remove_spawner(Registry&, Entity entity) {
    logger->trace("EntitySystem::remove_spawner");
    auto& spawner = get_component<Component::Spawner>(entity);
    spawner.callback.cancel();
}

void EntitySystem::remove_object(Registry& r, Entity entity) {
    logger->trace("EntitySystem::remove_object");
    if (auto* basicInfo = try_get_component<Component::BasicInfo>(entity); basicInfo && basicInfo->id) {
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

bool EntitySystem::is_valid(Entity entity) const {
    return registry.valid(entity);
}

void EntitySystem::register_name(Registry&, Entity entity) {
    //logger->trace("EntitySystem::register_name");
    const auto& basicInfo = get_component<Component::BasicInfo>(entity);
    if (basicInfo.name.size()) {
        name_to_entity.insert({basicInfo.name, entity});
    }
    if (basicInfo.id) {
        id_to_entity.insert({basicInfo.id, entity});
    }
}

void EntitySystem::unregister_name(Registry&, Entity entity) {
    logger->trace("EntitySystem::unregister_name");
    const auto& basic = get_component<Component::BasicInfo>(entity);
    if (basic.name.size()) {
        name_to_entity.erase(basic.name);
    }
    if (basic.id) {
        id_to_entity.erase(basic.id);
    }
}

Entity EntitySystem::get_entity_from_name(const std::string& name) const {
    auto res = name_to_entity.find(name);
    if (res != name_to_entity.end())
        return res->second;
    return entt::null;
}

Entity EntitySystem::get_entity_from_id(uint16_t id) const {
    auto res = id_to_entity.find(id);
    if (res != id_to_entity.end())
        return res->second;
    return entt::null;
}

void EntitySystem::stop() {
    std::lock_guard<std::recursive_mutex> lock(access);
    work_queue.kill();
    registry.on_construct<Component::Position>().disconnect<&Nearby::add_entity>(&nearby);
    registry.on_destroy<Component::Position>().disconnect<&EntitySystem::remove_object>(this);
    registry.on_construct<Component::BasicInfo>().disconnect<&EntitySystem::register_name>(this);
    registry.on_destroy<Component::BasicInfo>().disconnect<&EntitySystem::unregister_name>(this);
}

bool EntitySystem::dispatch_packet(Entity entity, std::unique_ptr<RoseCommon::CRosePacket>&& packet) {
    if (!packet) {
        return false;
    }
    if (!dispatcher.is_supported(*packet.get())) {
        return false;
    }
    add_task([entity, packet = std::move(packet)](EntitySystem& entitySystem) mutable {
        entitySystem.dispatcher.dispatch(std::move(packet), entitySystem, std::move(entity));
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

void EntitySystem::send_nearby(Entity entity, const RoseCommon::CRosePacket& packet) const {
    registry.view<const Component::Client>().each([entity, this, &packet](auto other, const auto &client_ptr) {
        if (!nearby.is_nearby(*this, entity, other)) return;
        if (auto client = client_ptr.client.lock()) {
            client->send(packet);
        }
    });
}

void EntitySystem::send_nearby_except_me(Entity entity, const RoseCommon::CRosePacket& packet) const {
    registry.view<const Component::Client>().each([entity, this, &packet](auto other, const auto &client_ptr) {
        if (other != entity) {
            if (!nearby.is_nearby(*this, entity, other)) return;
            if (auto client = client_ptr.client.lock()) {
                client->send(packet);
            }
        }
    });
}

void EntitySystem::send_to(Entity entity, const RoseCommon::CRosePacket& packet, bool force) const {
    if (const auto client_ptr = registry.try_get<Component::Client>(entity)) {
        if (auto client = client_ptr->client.lock()) {
            client->send(packet, force);
        }
    }
}

void EntitySystem::send_to_entity(Entity entity, Entity other) const {
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

void EntitySystem::delete_entity(Entity entity) {
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

void EntitySystem::update_position(Entity entity, float x, float y) {
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
    std::vector<Entity> to_remove;
    std::vector<Entity> to_add;
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

void EntitySystem::teleport_entity(Entity entity, float x, float y, uint16_t map_id) {
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
        const auto& basic = get_component<Component::BasicInfo>(entity);
        send_to_char_server(RoseCommon::Packet::IscClientStatus::create(basic.id,
                            RoseCommon::Packet::IscClientStatus::Status::SWITCHING), basic.charId);
        nearby.remove_entity(registry, entity);
        pos.x = x;
        pos.y = y;
        pos.map = map_id;
        save_character(entity);
        if (const auto client_ptr = registry.try_get<Component::Client>(entity)) {
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

std::vector<Entity> EntitySystem::get_nearby(Entity entity) const {
    const auto res = nearby.get_nearby(*this, entity);
    for (auto en : res) {
        update_command(*const_cast<EntitySystem*>(this), en);
    }
    return res;
}

std::future<Entity> EntitySystem::load_character(uint32_t charId, uint16_t access_level, uint32_t sessionId, std::weak_ptr<CMapClient> client) {
    std::promise<Entity> promise;
    std::future result = promise.get_future();

    add_task([charId, access_level, sessionId, client, promise = std::move(promise)](EntitySystem& self) mutable {
        using namespace Component;
        auto conn = Core::connectionPool.getConnection<Core::Osirose>();
        Core::CharacterTable characters{};
        Core::InventoryTable inventoryTable{};
        Core::SkillTable skillsTable{};

        auto charRes = conn(sqlpp::select(sqlpp::count(characters.id), sqlpp::all_of(characters))
                              .from(characters).where(characters.id == charId));

        if (static_cast<long>(charRes.front().count) != 1L) {
            promise.set_value(entt::null);
            return;
        }
        const auto& charRow = charRes.front();

        auto entity = self.registry.create();

        self.registry.emplace<BasicInfo>(
            entity,
            charRow.name, // name
            static_cast<uint16_t>(self.idManager.get_free_id()), // map id
            static_cast<uint32_t>(sessionId), // session id
            static_cast<int32_t>(sessionId), // team id
            static_cast<uint16_t>(charRow.job), // job
            static_cast<uint32_t>(charRow.statPoints), // stat points
            static_cast<uint32_t>(charRow.skillPoints), // skill points
            static_cast<uint16_t>(charRow.pkFlag), // pk flag
            static_cast<uint8_t>(charRow.stone), // stone
            static_cast<uint32_t>(charId) // char id
        );

        self.registry.emplace<Client>(
            entity,
            client, // cmapclient
            access_level // access level
        );

        self.registry.emplace<ComputedValues>(entity);

        auto& faction = self.registry.emplace<Faction>(
            entity,
            static_cast<uint8_t>(charRow.factionid), // id
            static_cast<uint8_t>(charRow.factionRank), // rank
            static_cast<uint8_t>(charRow.fame) // fame
        );
        faction.factionFame[0] = charRow.factionFame1;
        faction.factionFame[1] = charRow.factionFame2;
        faction.points[0] = charRow.factionPoints1;
        faction.points[1] = charRow.factionPoints2;
        faction.points[2] = charRow.factionPoints3;

        self.registry.emplace<CharacterGraphics>(
            entity,
            static_cast<uint8_t>(charRow.face), // face
            static_cast<uint8_t>(charRow.hair), // hair
            static_cast<uint8_t>(charRow.race) // race
        );

        self.registry.emplace<Guild>(
            entity,
            static_cast<uint32_t>(charRow.clanid), // id
            static_cast<uint16_t>(charRow.clanContribution), // contribution
            static_cast<uint8_t>(charRow.clanRank) // rank
        );

        self.registry.emplace<Hotbar>(entity);

        auto invRes =
          conn(sqlpp::select(sqlpp::all_of(inventoryTable)).from(inventoryTable)
            .where(inventoryTable.charId == charId and
               (inventoryTable.storageType == "inventory" or inventoryTable.storageType == "wishlist")));

        auto& wishlist = self.registry.emplace<Wishlist>(entity);
        auto& inventory = self.registry.emplace<Inventory>(entity);
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
            item.is_zuly = false;
            auto to_emplace = self.load_item(row.itemtype, row.itemid, item);
            if (is_inventory) {
                inventory.items[row.slot] = to_emplace;
            } else {
                wishlist.items[row.slot] = to_emplace;
            }
        }

        self.registry.emplace<Level>(
            entity,
            static_cast<uint16_t>(charRow.level), // level
            static_cast<uint64_t>(charRow.exp), // xp
            static_cast<uint64_t>(charRow.penaltyExp) // penalty xp
        );

        self.registry.emplace<Life>(
            entity,
            static_cast<int32_t>(charRow.currentHp <= 0 ? charRow.maxHp / 3 : charRow.currentHp), // current hp
            static_cast<int32_t>(charRow.maxHp) // max hp
        );

        self.registry.emplace<Magic>(
            entity,
            static_cast<int32_t>(charRow.currentMp <= 0 ? charRow.maxMp / 3 : charRow.currentMp), // current mp
            static_cast<int32_t>(charRow.maxMp) // max mp
        );

        self.registry.emplace<Position>(
            entity,
            static_cast<float>(charRow.x), // x
            static_cast<float>(charRow.y), // y
            static_cast<uint16_t>(0), // z
            static_cast<uint16_t>(charRow.reviveMap), // spawn point
            static_cast<uint16_t>(charRow.map) // map
        );

        auto skillRes =
          conn(sqlpp::select(skillsTable.id, skillsTable.level).from(skillsTable).where(skillsTable.charId == charId));
        auto& skills = self.registry.emplace<Skills>(entity);
        for (const auto& [i, row] : Core::enumerate(skillRes)) {
            skills.skills[i].set_id(row.id);
            skills.skills[i].set_level(row.level);
        }

        self.registry.emplace<Stamina>(
            entity,
            static_cast<uint16_t>(charRow.stamina), // current stamina
            static_cast<uint16_t>(charRow.stamina) // max stamina
        );

        self.registry.emplace<Stats>(
            entity,
            static_cast<uint16_t>(charRow.str), //  str
            static_cast<uint16_t>(charRow.dex), // dex
            static_cast<uint16_t>(charRow.int_), // intel
            static_cast<uint16_t>(charRow.con), // concentration
            static_cast<uint16_t>(charRow.charm), // charm
            static_cast<uint16_t>(charRow.sense), // sense
            static_cast<uint8_t>(100), // body size
            static_cast<uint8_t>(100) // head size
        );

        self.registry.emplace<StatusEffects>(entity);

        promise.set_value(entity);
    });
    return result;
}

void EntitySystem::save_character(Entity character) {
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
            characters.currentHp = life.hp,
            characters.maxHp = life.maxHp,
            characters.currentMp = magic.mp,
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
                if (itemDef.type != static_cast<uint32_t>(row.itemtype)
                    || itemDef.id != static_cast<uint32_t>(row.itemid)
                    || item.count != static_cast<uint32_t>(row.amount)
                    || item.refine != static_cast<uint32_t>(row.refine)
                    || item.gemOpt != static_cast<uint32_t>(row.gemOpt)
                    || item.hasSocket != static_cast<uint32_t>(row.socket)) {
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
                inventory.durability = item.durability,
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
                inventory.durability = item.durability,
                inventory.refine = item.refine,
                inventory.gemOpt = item.gemOpt,
                inventory.socket = static_cast<int>(item.hasSocket),
                inventory.slot = it,
                inventory.charId = basicInfo.charId
            ));
        }
    });
}

Entity EntitySystem::create_item(uint8_t type, uint16_t id, uint32_t count, uint8_t itemRefine, uint8_t itemDura, uint8_t itemSocket) {
    using namespace Component;

    const auto &itemDb = RoseCommon::ItemDatabase::getInstance();
    if (!itemDb.itemExists(type, id)) {
        logger->warn("No item {} {} in db", type, id);
        return entt::null;
    }

    auto entity = registry.create();

    const auto& def = itemDb.getItemDef(type, id);

    registry.emplace<Item>(
        entity,
        false, // is created
        false, // is zuly
        static_cast<uint16_t>(1000), // life
        static_cast<uint8_t>(std::min(int(itemDura), 120)), // durability
        (itemSocket > 0), // has socket
        false, // is appraised
        static_cast<uint8_t>(std::min(int(itemRefine), 9)), // refine
        static_cast<uint32_t>(count), // count
        static_cast<uint16_t>(0), // gem opt
        static_cast<uint32_t>(1000) // price
    );

    registry.emplace<RoseCommon::ItemDef>(entity, def);

    auto& lua = registry.emplace<ItemLua>(entity);
    lua.api = lua_loader.get_lua_item(type, id);
    if (const auto tmp = lua.api.lock(); tmp) {
        tmp->on_init();
    }

    return entity;
}

Entity EntitySystem::create_zuly(int64_t zuly) {
    using namespace Component;
    auto entity = registry.create();

    registry.emplace<Item>(
        entity,
        false, // is created
        true, // is zuly
        static_cast<uint16_t>(0), // life
        static_cast<uint8_t>(0), // durability
        false, // has socket
        true, // is appraised
        static_cast<uint8_t>(0), // refine
        static_cast<uint32_t>(zuly), // count
        static_cast<uint16_t>(0), // gem opt
        static_cast<uint32_t>(0) // price
    );

    return entity;
}

Entity EntitySystem::load_item(uint8_t type, uint16_t id, Component::Item item) {
    auto entity = create_item(type, id);
    if (entity == entt::null) {
        return entt::null;
    }
    registry.replace<Component::Item>(entity, item);
    return entity;
}

void EntitySystem::save_item([[maybe_unused]] Entity item, [[maybe_unused]] Entity owner) const {

}

Entity EntitySystem::create_npc(int quest_id, int npc_id, int map_id, float x, float y, float z, float angle) {
    logger->trace("EntitySystem::create_npc");
    using namespace Component;

    auto entity = registry.create();

    const auto id = idManager.get_free_id();
    registry.emplace<BasicInfo>(
        entity,
        "", // name
        static_cast<uint16_t>(id), // map id
        static_cast<uint32_t>(id), // world id
        static_cast<int32_t>(0), // team id
        static_cast<uint16_t>(0), // job
        static_cast<uint32_t>(0), // stat points
        static_cast<uint32_t>(0), // skill points
        static_cast<uint16_t>(0), // pk flag
        static_cast<uint8_t>(0), // stone
        static_cast<uint32_t>(0) // char id
    );

    auto ptr = lua_loader.get_data(npc_id);
    auto data = ptr.lock();
    if(!data)
        logger->warn("EntitySystem::create_mob unable to get npc data for {}", npc_id);

    registry.emplace<ComputedValues>(
        entity,
        RoseCommon::Command::STOP, // command
        RoseCommon::MoveMode::WALK, // move mode
        static_cast<uint16_t>(0) // run speed
    );

    const auto& level = registry.emplace<Level>(
        entity,
        static_cast<uint16_t>(data && data->get_level() > 0 ? data->get_level() : 1), // current level,
        static_cast<uint64_t>(0), // xp
        static_cast<uint64_t>(0) // penalty xp
    );

    const auto temp_hp = data ? data->get_hp() : 1;
    registry.emplace<Life>(
        entity,
        temp_hp * level.level, // current hp
        temp_hp * level.level // max hp
    );

    registry.emplace<Npc>(
        entity,
        static_cast<uint16_t>(npc_id), // id
        static_cast<uint16_t>(quest_id), // quest id
        angle, // angle
        static_cast<uint16_t>(0) // event status
    );

    registry.emplace<Position>(
        entity,
        x * 100.f, // x
        y * 100.f, // y
        static_cast<uint16_t>(z), // z
        static_cast<uint16_t>(0), // spawn point
        static_cast<uint16_t>(map_id) // map
    );

    // TODO: add lua

    return entity;
}

Entity EntitySystem::create_warpgate([[maybe_unused]] std::string alias,
    int id, int dest_map_id, float dest_x, float dest_y, float dest_z,
    float min_x, float min_y, float min_z,
    float max_x, float max_y, float max_z) {
    logger->trace("EntitySystem::create_warpgate");
    using namespace Component;

    auto entity = registry.create();

    registry.emplace<Warpgate>(
        entity,
        static_cast<int16_t>(id), // id
        min_x * 100.f, // min x
        min_y * 100.f, // min y
        static_cast<float>(min_z), // min z
        max_x * 100.f, // max x
        max_y * 100.f, // max y
        static_cast<float>(max_z), // max z
        static_cast<uint16_t>(dest_map_id) // destination map
    );

    registry.emplace<Destination>(
        entity,
        dest_x * 100.f, // x
        dest_y * 100.f, // y
        static_cast<int16_t>(dest_z * 100), // z
        static_cast<uint16_t>(0) // distance
    );

    // TODO: add lua

    return entity;
}

Entity EntitySystem::create_spawner([[maybe_unused]] std::string alias,
        int mob_id, int mob_count, int limit, int interval, int range, int map_id, float x, float y, float z) {
    logger->trace("EntitySystem::create_spawner");
    using namespace Component;

    auto entity = registry.create();

    auto& spawner = registry.emplace<Spawner>(
        entity,
        mob_id, // mob id
        mob_count, // mob count
        limit, // mob limit
        std::chrono::seconds(interval), // spawn interval
        range * 100, // range
        std::vector<Entity>{} // current spawned mobs
    );

    spawner.mobs.reserve(mob_count);

    registry.emplace<Position>(
        entity,
        x * 100.f, // x
        y * 100.f, // y
        static_cast<uint16_t>(z * 100), // z
        static_cast<uint16_t>(0), // spawn point
        static_cast<uint16_t>(map_id) // map id
    );

    spawner.callback = add_recurrent_timer(spawner.interval, [entity](EntitySystem& self) {
        if(self.is_loading()) return;
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

Entity EntitySystem::create_player_spawn(Component::PlayerSpawn::Type type, int map_id, float x, float y) {
    logger->trace("EntitySystem::create_player_spawn");
    using namespace Component;

    auto entity = registry.create();

    registry.emplace<PlayerSpawn>(
        entity,
        type // player spawn type
    );

    registry.emplace<Position>(
        entity,
        x * 100.f, // x
        y * 100.f, // y
        static_cast<uint16_t>(0), // we don't care about Z for now
        static_cast<uint16_t>(0), // spawn point
        static_cast<uint16_t>(map_id) // map id
    );

    return entity;
}

Entity EntitySystem::create_mob(Entity spawner) {
    //logger->trace("EntitySystem::create_mob");
    using namespace Component;

    auto entity = registry.create();

    const auto& spawn = get_component<Spawner>(spawner);
    const auto& spos = get_component<Position>(spawner);

    auto ptr = lua_loader.get_data(spawn.mob_id);
    auto data = ptr.lock();
    if(!data)
        logger->warn("EntitySystem::create_mob unable to get mob data for {}", spawn.mob_id);

    const auto id = idManager.get_free_id();
    registry.emplace<BasicInfo>(
        entity,
        "", // name
        static_cast<uint16_t>(id), // map id
        static_cast<uint32_t>(id), // world id
        static_cast<int32_t>(-1), // team id
        static_cast<uint16_t>(0), // job
        static_cast<uint32_t>(0), // stat points
        static_cast<uint32_t>(0), // skill points
        static_cast<uint16_t>(0), // pk flag
        static_cast<uint8_t>(0), // stone
        static_cast<uint32_t>(0) // char id
    );

    const auto& level = registry.emplace<Level>(
        entity,
        static_cast<uint16_t>(data && data->get_level() > 0 ? data->get_level() : 1), // level
        static_cast<uint64_t>(0), // xp
        static_cast<uint64_t>(0) // penalty xp
    );

    auto [x, y] = Core::Random::getInstance().random_in_circle(spos.x, spos.y, static_cast<float>(spawn.range));

    registry.emplace<Position>(
        entity,
        static_cast<float>(x), // x
        static_cast<float>(y), // y
        spos.z, // z
        static_cast<uint16_t>(0), // spawn
        spos.map // map
    );

    registry.emplace<ComputedValues>(
        entity,
        RoseCommon::Command::STOP, // command
        RoseCommon::MoveMode::WALK, // move mode
        static_cast<uint16_t>(data ? data->get_run_speed() : 0), // run speed
        static_cast<uint16_t>(data ? data->get_attack_spd() : 0) // attack speed
    );

    auto temp_hp = data ? data->get_hp() : 1;
    registry.emplace<Life>(
        entity,
        temp_hp * level.level, // current life
        temp_hp * level.level // max life
    );

    registry.emplace<Magic>(
        entity,
        static_cast<int32_t>(0), // current mp
        static_cast<int32_t>(0) // max mp
    );

    registry.emplace<Mob>(
        entity,
        static_cast<uint16_t>(spawn.mob_id), // id
        static_cast<uint16_t>(0) // quest
    );

    registry.emplace<Owner>(
        entity,
        spawner // owner
    );

    auto& lua = registry.emplace<NpcLua>(entity);
    lua.api = lua_loader.get_lua_npc(spawn.mob_id);
    lua.data = ptr;

    return entity;
}

void EntitySystem::send_to_maps(const RoseCommon::CRosePacket& packet, const std::vector<uint16_t>& maps) const {
    server->send_to_maps(packet, maps, 0);
}

void EntitySystem::send_to_char_server(const RoseCommon::CRosePacket& packet, uint32_t originator) const {
    server->send_to_maps(packet, {0}, originator);
}

void EntitySystem::send_to_chars(const RoseCommon::CRosePacket& packet, const std::vector<std::string>& chars) const {
    server->send_to_chars(packet, chars);
}

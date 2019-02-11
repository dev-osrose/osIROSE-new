#pragma once

#include <functional>
#include <chrono>
#include <queue>
#include <unordered_map>
#include <mutex>
#include <string>

#include "dataconsts.h"
#include "logconsole.h"
#include "mwsrqueue.h"
#include "fire_once.h"
#include "id_manager.h"
#include "components/item.h"
#include "components/player_spawn.h"
#include "itemdb.h"
#include "timed_callbacks.h"
#include "packet_dispatcher.h"
#include "nearby.h"
#include "lua_loader.h"

using namespace std::chrono_literals;

class CMapClient;
class CMapServer;

class EntitySystem {
    public:
        EntitySystem(uint16_t map_id, CMapServer* server, std::chrono::milliseconds maxTimePerUpdate = 50ms);
        EntitySystem(const EntitySystem&) = delete;
        EntitySystem(EntitySystem&&) = default;
        ~EntitySystem() = default;

        void run();
        void stop();
    
        uint16_t get_free_id();

        bool dispatch_packet(RoseCommon::Entity entity, std::unique_ptr<RoseCommon::CRosePacket>&& packet);

        template <typename T>
        void register_dispatcher(std::function<void(EntitySystem&, RoseCommon::Entity, const T&)>&& func);

        template <typename Func>
        void add_task(Func&& task);
    
        bool is_valid(RoseCommon::Entity entity) const;

        RoseCommon::Entity load_character(uint32_t charId, uint16_t access_level, uint32_t sessionId, std::weak_ptr<CMapClient> client);
        void save_character(RoseCommon::Entity);

        RoseCommon::Entity load_item(uint8_t type, uint16_t id, Component::Item);
        void save_item(RoseCommon::Entity item, RoseCommon::Entity owner) const;
        RoseCommon::Entity create_item(uint8_t type, uint16_t id, uint32_t count = 1);
        RoseCommon::Entity create_zuly(int64_t zuly);

        RoseCommon::Entity create_npc(int quest_id, int npc_id, int map_id, float x, float y, float z, float angle);
        RoseCommon::Entity create_warpgate(std::string alias, int id, int dest_map_id, float dest_x, float dest_y, float dest_z, float min_x, float min_y, float min_z, float max_x, float max_y, float max_z);
        RoseCommon::Entity create_spawner(std::string alias, int mob_id, int mob_count, int limit, int interval, int range, int map_id, float x, float y, float z);
        RoseCommon::Entity create_mob(RoseCommon::Entity spawner);
        RoseCommon::Entity create_player_spawn(Component::PlayerSpawn::Type type, int map_id, float x, float y);
        

        template <typename T>
        const T& get_component(RoseCommon::Entity entity) const;
        template <typename T>
        T& get_component(RoseCommon::Entity entity);
        template <typename T>
        const T* try_get_component(RoseCommon::Entity entity) const;
        template <typename T>
        T* try_get_component(RoseCommon::Entity entity);
        template <typename T>
        void remove_component(RoseCommon::Entity entity);
        template <typename T>
        T& add_component(RoseCommon::Entity entity);
        template <typename T>
        void add_component(RoseCommon::Entity entity, T&& comp);
        template <typename T>
        T& add_or_replace_component(RoseCommon::Entity entity);
        template <typename T>
        bool has_component(RoseCommon::Entity entity) const;
    
        template <typename T>
        auto item_to_equipped(RoseCommon::Entity entity) const;
        template <typename T>
        auto item_to_header(RoseCommon::Entity entity) const;
        template <typename T>
        auto item_to_data(RoseCommon::Entity entity) const;
        template <typename T>
        auto item_to_item(RoseCommon::Entity entity) const;

        void send_map(const RoseCommon::CRosePacket& packet) const;
        void send_nearby(RoseCommon::Entity entity, const RoseCommon::CRosePacket& packet) const;
        void send_nearby_except_me(RoseCommon::Entity entity, const RoseCommon::CRosePacket& packet) const;
        void send_to(RoseCommon::Entity entity, const RoseCommon::CRosePacket& packet, bool force = false) const;

        void send_to_entity(RoseCommon::Entity entity, RoseCommon::Entity other) const;

        void send_to_maps(const RoseCommon::CRosePacket& packet, const std::vector<uint16_t>& maps) const;
        void send_to_chars(const RoseCommon::CRosePacket& packet, const std::vector<std::string>& chars) const;

        void delete_entity(RoseCommon::Entity entity);

        void update_position(RoseCommon::Entity entity, float x, float y);
        void teleport_entity(RoseCommon::Entity entity, float x, float y, uint16_t map_id);
    
        // returns a sorted vector
        std::vector<RoseCommon::Entity> get_nearby(RoseCommon::Entity entity) const;
    
        RoseCommon::Entity get_entity_from_name(const std::string& name) const;
        RoseCommon::Entity get_entity_from_id(uint16_t id) const;
        RoseCommon::Entity get_entity_from_tag(uint32_t tag) const;

        uint16_t get_world_time() const;

        template <class Rep, class Period>
        TimedCallbacks::Wrapper add_timer(const std::chrono::duration<Rep, Period>& timeout, Core::fire_once<void(EntitySystem&)>&& callback);
        template <class Rep, class Period>
        TimedCallbacks::Wrapper add_recurrent_timer(const std::chrono::duration<Rep, Period>& timeout, std::function<void(EntitySystem&)> callback);

        template <typename... Components>
        auto get_entities_with_components();
        
        bool is_loading() const { return loading; }

    private:
        void register_name(RoseCommon::Registry&, RoseCommon::Entity entity);
        void unregister_name(RoseCommon::Registry&, RoseCommon::Entity entity);
        void remove_object(RoseCommon::Registry&, RoseCommon::Entity entity);
        void remove_spawner(RoseCommon::Registry&, RoseCommon::Entity entity);
    
        bool loading;
        std::chrono::steady_clock::time_point prevTime;
        Core::MWSRQueue<std::deque<Core::fire_once<void(EntitySystem&)>>> work_queue;
        std::unordered_map<std::string, RoseCommon::Entity> name_to_entity;
        std::unordered_map<uint16_t, RoseCommon::Entity> id_to_entity;
        std::unordered_map<uint32_t, RoseCommon::Entity> tag_to_entity;
        RoseCommon::Registry registry;
        std::shared_ptr<spdlog::logger> logger;
        std::chrono::milliseconds maxTimePerUpdate;
        std::recursive_mutex access;
        IdManager idManager;
        TimedCallbacks timers;
        PacketDispatcher dispatcher;
        Nearby nearby;
        LuaLoader lua_loader;
        CMapServer *server;
};


// ----------------------------- template implementations ----------------------------------
template <typename T>
void EntitySystem::register_dispatcher(std::function<void(EntitySystem&, RoseCommon::Entity, const T&)>&& func) {
    dispatcher.add_dispatcher(T::PACKET_ID, std::move(func));
}

template <typename Func>
void EntitySystem::add_task(Func&& task) {
    static_assert(std::is_invocable_v<Func, EntitySystem&>, "task should be of the form void(*)(EntitySystem&)");
    work_queue.push_back(std::forward<Func>(task));
}

template <typename T>
const T& EntitySystem::get_component(RoseCommon::Entity entity) const {
    return registry.get<T>(entity);
}
    
template <typename T>
T& EntitySystem::get_component(RoseCommon::Entity entity) {
    return registry.get<T>(entity);
}

template <typename T>
const T* EntitySystem::try_get_component(RoseCommon::Entity entity) const {
    return registry.try_get<T>(entity);
}
    
template <typename T>
T* EntitySystem::try_get_component(RoseCommon::Entity entity) {
    return registry.try_get<T>(entity);
}
        
template <typename T>
void EntitySystem::remove_component(RoseCommon::Entity entity) {
    if (has_component<T>(entity)) {
        registry.remove<T>(entity);
    }
}
    
template <typename T>
T& EntitySystem::add_component(RoseCommon::Entity entity) {
    return registry.assign<T>(entity);
}

template <typename T>
void EntitySystem::add_component(RoseCommon::Entity entity, T&& comp) {
    registry.assign<T>(entity, std::forward<T>(comp));
}

template <typename T>
T& EntitySystem::add_or_replace_component(RoseCommon::Entity entity) {
    return registry.assign_or_replace<T>(entity);
}

template <typename T>
bool EntitySystem::has_component(RoseCommon::Entity entity) const {
    return registry.has<T>(entity);
}
    
template <typename T>
auto EntitySystem::item_to_equipped(RoseCommon::Entity entity) const {
    if (entity == entt::null) {
        return typename T::EquippedItem{};
    }
    const auto& item = get_component<Component::Item>(entity);
    const auto& data = get_component<RoseCommon::ItemDef>(entity);

    typename T::EquippedItem itemPacket;
    itemPacket.set_id(data.id);
    itemPacket.set_gem_opt(item.gemOpt);
    itemPacket.set_socket(item.hasSocket);
    itemPacket.set_grade(item.refine);
    return itemPacket;
}
    
template <typename T>
auto EntitySystem::item_to_header(RoseCommon::Entity entity) const {
    if (entity == entt::null) {
        return typename T::Header{};
    }
    const auto& item = get_component<Component::Item>(entity);
    const auto* data = try_get_component<RoseCommon::ItemDef>(entity);
        
    typename T::Header header;
    header.set_isCreated(item.isCreated);
    if (data) {
        header.set_id(data->id);
        header.set_type(RoseCommon::to_underlying(data->type));
    } else {
        header.set_id(0);
        header.set_type(RoseCommon::to_underlying(RoseCommon::ItemType::ZULY));
    }
        
    return header;
}
    
template <typename T>
auto EntitySystem::item_to_data(RoseCommon::Entity entity) const {
    if (entity == entt::null) {
        return typename T::Data{};
    }
    const auto& item = get_component<Component::Item>(entity);
    const auto* itemDef = try_get_component<RoseCommon::ItemDef>(entity);
        
    typename T::Data data;
    if (!itemDef || itemDef->is_stackable) {
        data.set_count(item.count);
    } else {
        data.set_refine(item.refine);
        data.set_isAppraised(item.isAppraised);
        data.set_hasSocket(item.hasSocket);
        data.set_life(item.life);
        data.set_durability(item.durability);
        data.set_gem_opt(item.gemOpt);
    }
    return data;
}
    
template <typename T>
auto EntitySystem::item_to_item(RoseCommon::Entity entity) const {
    typename T::Item item{};
    if (entity == entt::null) {
        return item;
    }
    item.set_header(item_to_header<T>(entity));
    item.set_data(item_to_data<T>(entity));
    return item;
}

template <class Rep, class Period>
TimedCallbacks::Wrapper EntitySystem::add_timer(const std::chrono::duration<Rep, Period>& timeout, Core::fire_once<void(EntitySystem&)>&& callback) {
    return timers.add_callback(timeout, [this, callback = std::move(callback)]() mutable {
        add_task(std::move(callback));
    });
}

template <class Rep, class Period>
TimedCallbacks::Wrapper EntitySystem::add_recurrent_timer(const std::chrono::duration<Rep, Period>& timeout, std::function<void(EntitySystem&)> callback) {
    return timers.add_recurrent_callback(timeout, [this, callback = std::move(callback)]() mutable {
        add_task(callback);
    });
}

template <typename... Components>
auto EntitySystem::get_entities_with_components() {
    return registry.view<Components...>();
}

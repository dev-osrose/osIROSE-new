#pragma once

#include <functional>
#include <chrono>
#include <queue>
#include <unordered_map>
#include <string>

#include "dataconsts.h"
#include "logconsole.h"
#include "mwsrqueue.h"
#include "fire_once.h"
#include "id_manager.h"
#include "components/item.h"
#include "itemdb.h"
#include "timed_callbacks.h"
#include "packet_dispatcher.h"
#include "nearby.h"

using namespace std::chrono_literals;

class CMapClient;

class EntitySystem {
    public:
        EntitySystem(std::chrono::milliseconds maxTimePerUpdate = 50ms);

        void run();
        void stop();
    
        bool dispatch_packet(RoseCommon::Entity entity, std::unique_ptr<RoseCommon::CRosePacket>&& packet);

        template <typename T>
        void register_dispatcher(std::function<void(EntitySystem&, RoseCommon::Entity, const T&)>&& func);

        template <typename Func>
        void add_task(Func&& task);

        RoseCommon::Entity load_character(uint32_t charId, bool platinium, uint32_t sessionId, std::weak_ptr<CMapClient> client);
        void save_character(RoseCommon::Entity);

        RoseCommon::Entity load_item(uint8_t type, uint16_t id, Component::Item);
        void save_item(RoseCommon::Entity item, RoseCommon::Entity owner) const;
        RoseCommon::Entity create_item(uint8_t type, uint16_t id);

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
        auto item_to_equipped(RoseCommon::Entity entity) const;
        template <typename T>
        auto item_to_header(RoseCommon::Entity entity) const;
        template <typename T>
        auto item_to_data(RoseCommon::Entity entity) const;
        template <typename T>
        auto item_to_item(RoseCommon::Entity entity) const;
        template <typename T>
        auto item_to_bullet(RoseCommon::Entity entity) const;

        void send_map(const RoseCommon::CRosePacket& packet) const;
        void send_nearby(RoseCommon::Entity entity, const RoseCommon::CRosePacket& packet) const;
        void send_nearby_except_me(RoseCommon::Entity entity, const RoseCommon::CRosePacket& packet) const;
        void send_to(RoseCommon::Entity entity, const RoseCommon::CRosePacket& packet) const;

        void delete_entity(RoseCommon::Entity entity);

        void update_position(RoseCommon::Entity entity, float x, float y);
    
        // returns a sorted vector
        std::vector<RoseCommon::Entity> get_nearby(RoseCommon::Entity entity) const;
    
        RoseCommon::Entity get_entity_from_name(const std::string& name) const;

        uint16_t get_world_time() const;

        template <class Rep, class Period>
        void add_timer(const std::chrono::duration<Rep, Period>& timeout, Core::fire_once<void(EntitySystem&)>&& callback);
        template <class Rep, class Period>
        void add_recurrent_timer(const std::chrono::duration<Rep, Period>& timeout, std::function<void(EntitySystem&)> callback);

    private:
        void register_name(RoseCommon::Registry&, RoseCommon::Entity entity);
        void unregister_name(RoseCommon::Registry&, RoseCommon::Entity entity);
        void remove_object(RoseCommon::Registry&, RoseCommon::Entity entity);
    
        Core::MWSRQueue<std::deque<Core::fire_once<void(EntitySystem&)>>> work_queue;
        std::unordered_map<std::string, RoseCommon::Entity> name_to_entity;
        RoseCommon::Registry registry;
        std::shared_ptr<spdlog::logger> logger;
        std::chrono::milliseconds maxTimePerUpdate;
        std::mutex access;
        IdManager idManager;
        TimedCallbacks timers;
        PacketDispatcher dispatcher;
        Nearby nearby;
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
    registry.remove<T>(entity);
}
    
template <typename T>
T& EntitySystem::add_component(RoseCommon::Entity entity) {
    return registry.assign<T>(entity);
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
    const auto& data = get_component<RoseCommon::ItemDef>(entity);
        
    typename T::Header header;
    header.set_isCreated(item.isCreated);
    header.set_id(data.id);
    header.set_type(data.type);
        
    return header;
}
    
template <typename T>
auto EntitySystem::item_to_data(RoseCommon::Entity entity) const {
    if (entity == entt::null) {
        return typename T::Data{};
    }
    const auto& item = get_component<Component::Item>(entity);
    const auto& itemDef = get_component<RoseCommon::ItemDef>(entity);
        
    typename T::Data data;
    if (itemDef.type == RoseCommon::ItemType::CONSUMABLE) {
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

template <typename T>
auto EntitySystem::item_to_bullet(RoseCommon::Entity entity) const {
    typename T::BulletItem item{};
    if (entity == entt::null) {
        return item;
    }
    const auto& itemDef = get_component<RoseCommon::ItemDef>(entity);
    item.set_type(itemDef.type);
    item.set_id(itemDef.id);
    return item;
}

template <class Rep, class Period>
void EntitySystem::add_timer(const std::chrono::duration<Rep, Period>& timeout, Core::fire_once<void(EntitySystem&)>&& callback) {
    timers.add_callback(timeout, [this, callback = std::move(callback)]() mutable {
        add_task(std::move(callback));
    });
}

template <class Rep, class Period>
void EntitySystem::add_recurrent_timer(const std::chrono::duration<Rep, Period>& timeout, std::function<void(EntitySystem&)> callback) {
    timers.add_recurrent_callback(timeout, [this, callback = std::move(callback)]() mutable {
        add_task(callback);
    });
}

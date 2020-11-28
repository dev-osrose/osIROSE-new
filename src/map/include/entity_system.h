#pragma once

#include <functional>
#include <chrono>
#include <queue>
#include <unordered_map>
#include <mutex>
#include <string>
#include <future>

#include "entity.h"
#include "dataconsts.h"
#include "logconsole.h"
#include "id_manager.h"
#include "components/item.h"
#include "components/player_spawn.h"
#include "components/basic_info.h"
#include "itemdb.h"

class CMapClient;
class CMapServer;

class EntitySystem {
    public:
        EntitySystem();
        EntitySystem(const EntitySystem&) = delete;
        EntitySystem(EntitySystem&&) = default;
        ~EntitySystem() = default;

        Entity create_entity();
        bool is_valid(Entity entity) const;
        void delete_entity(Entity entity);

        template <typename T>
        const T& get_component(Entity entity) const;
        template <typename T>
        T& get_component(Entity entity);
        template <typename T>
        const T* try_get_component(Entity entity) const;
        template <typename T>
        T* try_get_component(Entity entity);
        template <typename T>
        void remove_component(Entity entity);
        template <typename T>
        T& add_component(Entity entity);
        template <typename T>
        void add_component(Entity entity, T&& comp);
        template <typename T>
        T& add_or_replace_component(Entity entity);
        template <typename T>
        bool has_component(Entity entity) const;
    
        template <typename... Components>
        auto get_entities_with_components();
        
    private:
        Registry registry;
        std::shared_ptr<spdlog::logger> logger;
};


template <typename T>
const T& EntitySystem::get_component(Entity entity) const {
    return registry.get<T>(entity);
}
    
template <typename T>
T& EntitySystem::get_component(Entity entity) {
    return registry.get<T>(entity);
}

template <typename T>
const T* EntitySystem::try_get_component(Entity entity) const {
    return registry.try_get<T>(entity);
}
    
template <typename T>
T* EntitySystem::try_get_component(Entity entity) {
    return registry.try_get<T>(entity);
}
        
template <typename T>
void EntitySystem::remove_component(Entity entity) {
    if (has_component<T>(entity)) {
        registry.remove<T>(entity);
    }
}
    
template <typename T>
T& EntitySystem::add_component(Entity entity) {
    return registry.emplace<T>(entity);
}

template <typename T>
void EntitySystem::add_component(Entity entity, T&& comp) {
    registry.emplace<T>(entity, std::forward<T>(comp));
}

template <typename T>
T& EntitySystem::add_or_replace_component(Entity entity) {
    return registry.emplace_or_replace<T>(entity);
}

template <typename T>
bool EntitySystem::has_component(Entity entity) const {
    return registry.has<T>(entity);
}
    
template <typename... Components>
auto EntitySystem::get_entities_with_components() {
    return registry.view<Components...>();
}

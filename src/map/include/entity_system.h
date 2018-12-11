#pragma once

#include <functional>
#include <chrono>
#include <queue>

#include "dataconsts.h"
#include "logconsole.h"
#include "mwsrqueue.h"
#include "id_manager.h"
#include "components/item.h"

using namespace std::chrono_literals;

class EntitySystem {
    public:
        EntitySystem(std::chrono::milliseconds maxTimePerUpdate = 50ms);
        ~EntitySystem();

        void update(std::chrono::milliseconds);

        void add_task(std::function<void(RoseCommon::Registry&, std::chrono::milliseconds)>&& task);

        RoseCommon::Entity load_character(uint32_t charId, bool platinium, uint32_t sessionId);
        void save_character(RoseCommon::Entity) const;

        RoseCommon::Entity load_item(uint8_t type, uint16_t id, Component::Item);
        void save_item(RoseCommon::Entity item, RoseCommon::Entity owner) const;
        RoseCommon::Entity create_item(uint8_t type, uint16_t id);

        template <typename T>
        const T& get_component(RoseCommon::Entity entity) const {
            return registry.get<T>(entity);
        }
	
	    template <typename T>
	    T& get_component(RoseCommon::entity entity) {
	        return registry.get<T>(entity);
        }

        template <typename T>
        const T* try_get_component(RoseCommon::Entity entity) const {
            return registry.try_get<T>(entity);
        }
    
        template <typename T>
        T* try_get_component(RoseCommon::Entity entity) {
            return registry.try_get<T>(entity);
        }
        
        template <typename T>
        void remove_component(RoseCommon::Entity entity) {
            registry.remove<T>(entity);
        }
    
        template <typename T>
        T& add_component(RoseCommon::Entity entity) {
            return registry.assign<T>(entity);
        }

    private:
        Core::MWSRQueue<std::deque<std::function<void(RoseCommon::Registry&, std::chrono::milliseconds)>>> work_queue;
        RoseCommon::Registry registry;
        std::shared_ptr<spdlog::logger> logger;
		std::chrono::milliseconds maxTimePerUpdate;
        std::mutex access;
        IdManager idManager;
};

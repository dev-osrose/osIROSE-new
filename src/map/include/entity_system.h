#pragma once

#include <functional>
#include <chrono>
#include <queue>

#include "dataconsts.h"
#include "logconsole.h"
#include "mwsrqueue.h"
#include "id_manager.h"
#include "components/item.h"
#include "itemdb.h"

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
	    T& get_component(RoseCommon::Entity entity) {
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
	
	template <typename T>
	auto item_to_equipped(RoseCommon::Entity entity) const {
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
    auto item_to_header(RoseCommon::Entity entity) const {
        const auto& item = get_component<Component::Item>(entity);
        const auto& data = get_component<RoseCommon::ItemDef>(entity);
        
        typename T::Header header;
        header.set_isCreated(item.isCreated);
        header.set_id(data.id);
        header.set_type(data.type);
        
        return header;
    }
    
    template <typename T>
    auto item_to_data(RoseCommon::Entity entity) const {
        const auto& item = get_component<Component::Item>(entity);
        
        typename T::Data data;
        if (item.count == 0) {
            data.set_refine(item.refine);
            data.set_isAppraised(item.isAppraised);
            data.set_hasSocket(item.hasSocket);
            data.set_life(item.life);
            data.set_durability(item.durability);
            data.set_gem_opt(item.gemOpt);
        } else {
            data.set_count(item.count);
        }
        return data;
    }
    
    template <typename T>
    auto item_to_item(RoseCommon::Entity entity) const {
        typename T::Item item;
        item.set_header(item_to_header<T>(entity));
        item.set_data(item_to_data<T>(entity));
        return item;
    }

    private:
        Core::MWSRQueue<std::deque<std::function<void(RoseCommon::Registry&, std::chrono::milliseconds)>>> work_queue;
        RoseCommon::Registry registry;
        std::shared_ptr<spdlog::logger> logger;
		std::chrono::milliseconds maxTimePerUpdate;
        std::mutex access;
        IdManager idManager;
};

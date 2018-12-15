#pragma once

#include <functional>
#include <chrono>
#include <queue>

#include "dataconsts.h"
#include "logconsole.h"
#include "mwsrqueue.h"
#include "fire_once.h"
#include "id_manager.h"
#include "components/item.h"
#include "itemdb.h"
#include "timed_callbacks.h"
#include "packet_dispatcher.h"

using namespace std::chrono_literals;

class EntitySystem {
    public:
        EntitySystem(std::chrono::milliseconds maxTimePerUpdate = 50ms);
        ~EntitySystem();

        void update(std::chrono::milliseconds);
	
	    bool dispatch_packet(RoseCommon::Entity entity, std::unique_ptr<RoseCommon::CRosePacket>&& packet) {
		    if (!packet) {
			return false;
		    }
		    if (!dispatcher.is_supported(*packet.get())) {
		        return false;
            	}
            add_task(std::move([this, entity, packet = std::move(packet)](RoseCommon::Registry& registry, std::chrono::milliseconds dt) mutable {
                dispatcher.dispatch(registry, entity, dt, std::move(packet));
            }));
            return true;
	    }

		template <typename Func>
        void add_task(Func&& task) {
			static_assert(std::is_invocable_v<Func, RoseCommon::Registry&, std::chrono::milliseconds>, "task should be of the form void(*)(RoseCommon::Registry&, std::chrono::milliseconds)");
			work_queue.push_back(std::forward<Func>(task));
		}

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
    auto item_to_header(RoseCommon::Entity entity) const {
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
    auto item_to_data(RoseCommon::Entity entity) const {
        if (entity == entt::null) {
            return typename T::Data{};
        }
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
        typename T::Item item{};
        if (entity == entt::null) {
            return item;
        }
        item.set_header(item_to_header<T>(entity));
        item.set_data(item_to_data<T>(entity));
        return item;
    }
	
    template <class Rep, class Period>
    void add_timer(const std::chrono::duration<Rep, Period>& timeout, Core::fire_once<void(RoseCommon::Registry&, std::chrono::milliseconds)>&& callback) {
        timers.add_callback(timeout, [this, callback = std::move(callback)]() mutable {
            add_task(std::move(callback));
        });
    }

    private:
        Core::MWSRQueue<std::deque<Core::fire_once<void(RoseCommon::Registry&, std::chrono::milliseconds)>>> work_queue;
        RoseCommon::Registry registry;
        std::shared_ptr<spdlog::logger> logger;
		std::chrono::milliseconds maxTimePerUpdate;
        std::mutex access;
        IdManager idManager;
	    TimedCallbacks timers;
        PacketDispatcher dispatcher;
};

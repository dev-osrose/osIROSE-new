#pragma once

#include "packetfactory.h"
#include "dataconsts.h"
#include <unordered_map>
#include <functional>
#include <type_traits>
#include <chrono>

class EntitySystem;

class PacketDispatcher {
    public:
    
        bool is_supported(const RoseCommon::CRosePacket& packet) {
            const RoseCommon::ePacketType type = packet.get_type();
            auto res = dispatcher.equal_range(type);
            return std::distance(res.first, res.second) > 0;
        }
    
        void dispatch(EntitySystem& entitySystem, RoseCommon::Entity entity, std::unique_ptr<RoseCommon::CRosePacket> packet) {
            if (!packet) {
                return;
            }
            const RoseCommon::ePacketType type = packet->get_type();
            auto res = dispatcher.equal_range(type);
            for (auto it = res.first; it != res.second; ++it) {
                it->second(entitySystem, entity, packet.get());
            }
        }
    
        template <typename PacketType, typename Func>
        void add_dispatcher(RoseCommon::ePacketType type, Func&& func) {
            static_assert(std::is_invocable<Func, EntitySystem&, RoseCommon::Entity, const PacketType&>::value,
                          "registering function must be of the form void(*)(EntitySystem&, RoseCommon::Entity, const PacketType&)");
            dispatcher.emplace(type, [func = std::forward<Func>(func)](EntitySystem& entitySystem, RoseCommon::Entity entity, const RoseCommon::CRosePacket* packet) mutable {
                PacketType *p = dynamic_cast<PacketType*>(packet);
                if (p == nullptr) {
                    return;
                }
                func(entitySystem, entity, *p);
            });
        }
    
    private:
        std::unordered_map<RoseCommon::ePacketType, std::function<void(EntitySystem&, RoseCommon::Entity, const RoseCommon::CRosePacket*)>> dispatcher;
};

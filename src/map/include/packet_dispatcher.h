#pragma once

#include "packetfactory.h"
#include "dataconsts.h"
#include <unordered_map>
#include <functional>
#include <type_traits>
#include <chrono>

class PacketDispatcher {
    public:
    
        bool is_supported(const RoseCommon::CRosePacket& packet) {
            const RoseCommon::ePacketType type = packet.get_type();
            auto res = dispatcher.equal_range(type);
            return std::distance(res.first, res.second) > 0;
        }
    
        void dispatch(RoseCommon::Registry& registry, RoseCommon::Entity entity, std::chrono::milliseconds dt, std::unique_ptr<RoseCommon::CRosePacket> packet) {
            if (!packet) {
                return;
            }
            const RoseCommon::ePacketType type = packet->get_type();
            auto res = dispatcher.equal_range(type);
            for (auto it = res.first; it != res.second; ++it) {
                it->second(registry, entity, dt, packet.get());
            }
        }
    
        template <typename PacketType, typename Func>
        void add_dispatcher(RoseCommon::ePacketType type, Func&& func) {
            static_assert(std::is_invocable<Func, RoseCommon::Registry&, RoseCommon::Entity, std::chrono::milliseconds, const PacketType&>::value,
                          "registering function must be of the form void(*)(RoseCommon::Registry&, RoseCommon::Entity, std::chrono::milliseconds, const PacketType&)");
            dispatcher.emplace(type, [func = std::forward<Func>(func)](RoseCommon::Registry& registry, RoseCommon::Entity entity, std::chrono::milliseconds dt, const RoseCommon::CRosePacket* packet) mutable {
                PacketType *p = dynamic_cast<PacketType*>(packet);
                if (p == nullptr) {
                    return;
                }
                func(registry, entity, dt, *p);
            });
        }
    
    private:
        std::unordered_map<RoseCommon::ePacketType, std::function<void(RoseCommon::Registry&, RoseCommon::Entity, std::chrono::milliseconds, const RoseCommon::CRosePacket*)>> dispatcher;
};

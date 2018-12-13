#pragma once

#include "packetfactory.h"
#include "dataconsts.h"
#include <unordered_map>
#include <functional>
#include <type_traits>

class PacketDispatcher {
    public:
    
        template <typename T>
        bool is_supported(T&& packet) {
            constexpr ePacketType type = template T::PACKET_ID;
            auto res = dispatcher.equal_range(type);
            return std::distance(res.first, res.second) > 0;
        }
    
        template <typename T>
        void dispatch(RoseCommon::Entity entity, T&& packet) {
            constexpr ePacketType type = template T::PACKET_ID;
            auto res = dispatcher.equal_range(type);
            for (auto it = res.first; it != res.second; ++it) {
                it->second(entity, packet);
            }
        }
    
        <template typename PacketType, typename Func>
        void add_dispatcher(ePacketType type, Func&& func) {
            static_assert(std::is_invocable<Func, RoseCommon::Entity, const PacketType&>::value, "registering function must be of the form void(*)(RoseCommon::Entity, const PacketType&)");
            dispatcher.emplace(type, [func = std::forward<Func>(func)](RoseCommon::Entity entity, const RoseCommon::CRosePacket& packet) mutable {
                PacketType *p = dynamic_cast<PacketType*>(&packet);
                if (p == nullptr) {
                    return;
                }
                func(entity, *p);
            });
        }
    
    private:
        std::unordered_map<ePacketType, std::function<void(RoseCommon::Entity, const RoseCommon::CRosePacket&)>> dispatcher;
};

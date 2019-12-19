#pragma once

#include "packetfactory.h"
#include <unordered_map>
#include <functional>
#include <type_traits>
#include <chrono>

namespace RoseCommon {

template <typename... Args>
class PacketDispatcher final {
    public:
    
        bool is_supported(const RoseCommon::CRosePacket& packet) const {
            const RoseCommon::ePacketType type = packet.get_type();
            auto res = dispatcher.equal_range(type);
            return std::distance(res.first, res.second) > 0;
        }

        void dispatch(std::unique_ptr<RoseCommon::CRosePacket>&& packet, Args&&... args) {
            if (!packet) {
                return;
            }
            const RoseCommon::ePacketType type = packet->get_type();
            auto res = dispatcher.equal_range(type);
            for (auto it = res.first; it != res.second; ++it) {
                it->second(packet.get(), std::forward<Args>(args)...);
            }
        }

        template <typename PacketType>
        void add_dispatcher(RoseCommon::ePacketType type, std::function<void(const PacketType&, Args...)>&& func) {
            dispatcher.emplace(type, [func = std::move(func)](const RoseCommon::CRosePacket* packet, Args&&... args) mutable {
                const PacketType *p = dynamic_cast<const PacketType*>(packet);
                if (p == nullptr) {
                    return;
                }
                func(*p, std::forward<Args>(args)...);
            });
        }

    private:
        std::unordered_multimap<RoseCommon::ePacketType, std::function<void(const RoseCommon::CRosePacket*, Args&&...)>> dispatcher;
};

}

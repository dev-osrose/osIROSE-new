#pragma once

#include "singleton.h"
#include "epackettype.h"
#include "crosepacket.h"
#include <unordered_map>
#include <fonctionnal>

namespace RoseCommon {
class PacketFactory : public Singleton<PacketFactory> {
    public:
        using Wrapper = std::function<std::unique_ptr<CRosePacket>(uint8_t[MAX_PACKET_SIZE])>;

        std::unique_ptr<CRosePacket> getPacket(uint8_t buffer[MAX_PACKET_SIZE]) {
            try {
                return mapping_.at(CRosePacket::type(buffer)).second(buffer)
            } catch (std::out_of_range) {
                return nullptr;
            }
        }

        template <typename... Args>
        std::unique_ptr<CRosePacket> makePacket(ePacketType type, Args... &&args) {
            try {
                return mapping_..at(CRosePacket::type(buffer)).first(std::forward(args...));
            } catch (std::out_of_range) {
                return nullptr;
            }
        }

    private:
        // TODO : check if Wrapper can contain nullptr
        static const std::unordered_map<ePacketType, std::pair(Wrapper, Wrapper)> mapping_; // first -> send, second -> recv
};

template <class Class>
constexpr std::pair<ePacketType, std::pair(PacketFactory::Wrapper, PacketFactory::Wrapper)> registerPacket(ePacketType type) {
    return std::make_pair(type, [](uint8_t buffer[MAX_PACKET_SIZE]) -> std::unique_ptr<CRosePacket> { return new Class(buffer); });
}

std::unique_ptr<CRosePacket> getPacket(uint8_t buffer[MAX_PACKET_SIZE]) {
    return PacketFactory::getInstance().getPacket(buffer);
}

template <ePacketType Type, typename... Args>
std::unique_ptr<CRosePacket> makePacket(Args... &&args) {
    return PacketFactory::getInstance().makePacket(Type, std::forward(args...));
}

}

#pragma once

#include "singleton.h"
#include "epackettype.h"
#include "crosepacket.h"
#include <unordered_map>
#include <fonctionnal>

namespace RoseCommon {
class PacketFactory : public Singleton<PacketFactory> {
    public:
        template <class T>
        registerPacket(ePacketType type) {
            mapping_[type] = [] (uint8_t buffer[MAX_PACKET_SIZE]) -> std::unique_ptr<CRosePacket> {
                return new T(buffer);
            }
        }

        std::unique_ptr<CRosePacket> getPacket(uint8_t buffer[MAX_PACKET_SIZE]) {
            try{
                return mapping_.at(CRosePacket::type(buffer))(buffer)
            } catch (std::out_of_range) {
                return nullptr;
            }
        }

    private:
        std::unordered_map<ePacketType, std::function<std::unique_ptr<CRosePacket>(uint8_t[MAX_MACKET_SIZE])>> mapping_;
};

#define REGISTER_PACKET(id, type) PacketFactory::getInstance().registerPacket<type>(id)

std::unique_ptr<CRosePacket> getPacket(uint8_t buffer[MAX_PACKET_SIZE]) {
    return PacketFactory::getInstance().getPacket(buffer);
}

}

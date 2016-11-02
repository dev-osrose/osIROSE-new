#include "packetfactory.h"

namespace RoseCommon {

std::unordered_map<ePacketType, std::function<std::unique_ptr<CRosePacket>(uint8_t[MAX_MACKET_SIZE])>> PacketFactory::mapping_ = {
    REGISTER_PACKET(ePacketType::),
};

}

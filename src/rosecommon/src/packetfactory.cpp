#include "packetfactory.h"
#include "entityComponents.h"

namespace RoseCommon {

std::unordered_map<ePacketType, std::function<std::unique_ptr<CRosePacket>(uint8_t[MAX_MACKET_SIZE])>> PacketFactory::mapping_ = {
    registerPacket<CliMouseCmd>(ePacketType::PAKCS_MOUSE_CMD),
};

}

uint16_t getId(Entity entity) {
    return entity.component<BasicInfo>()->id_;
}

std::string &getName(Entity entity) {
    return entity.component<BasicInfo>()->name_;
}

CMapClient* getClient(Entity entity) {
    if (!entity.component<SocketConnector>())
        return nullptr;
    return entity.component<SocketConnector>()->client_;
}


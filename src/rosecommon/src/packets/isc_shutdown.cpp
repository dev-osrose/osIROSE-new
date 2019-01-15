#include "isc_shutdown.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;



IscShutdown::IscShutdown() : CRosePacket(IscShutdown::PACKET_ID) {}

IscShutdown::IscShutdown(CRoseReader reader) : CRosePacket(reader) {
    if (!reader.get_uint8_t((uint8_t&)serverType)) {
        return;
    }
    if (!reader.get_int32_t(id)) {
        return;
    }
}

void IscShutdown::set_serverType(const Isc::ServerType serverType) {
    this->serverType = serverType;
}

Isc::ServerType IscShutdown::get_serverType() const {
    return serverType;
}

void IscShutdown::set_id(const int32_t id) {
    this->id = id;
}

int32_t IscShutdown::get_id() const {
    return id;
}

IscShutdown IscShutdown::create(const Isc::ServerType& serverType, const int32_t& id) {
    IscShutdown packet;
    packet.set_serverType(serverType);
    packet.set_id(id);
    return packet;
}

IscShutdown IscShutdown::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return IscShutdown(reader);
}

std::unique_ptr<IscShutdown> IscShutdown::allocate(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return std::make_unique<IscShutdown>(reader);
}

void IscShutdown::pack(CRoseBasePolicy& writer) const {
    if (!writer.set_uint8_t(serverType)) {
        return;
    }
    if (!writer.set_int32_t(id)) {
        return;
    }
}

constexpr size_t IscShutdown::size() {
    size_t size = 0;
    size += sizeof(Isc::ServerType); // serverType
    size += sizeof(int32_t); // id
    return size;
}


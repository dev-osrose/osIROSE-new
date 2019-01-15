#include "srv_remove_object.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;



SrvRemoveObject::SrvRemoveObject() : CRosePacket(SrvRemoveObject::PACKET_ID) {}

SrvRemoveObject::SrvRemoveObject(CRoseReader reader) : CRosePacket(reader) {
    if (!reader.get_uint16_t(id)) {
        return;
    }
}

void SrvRemoveObject::set_id(const uint16_t id) {
    this->id = id;
}

uint16_t SrvRemoveObject::get_id() const {
    return id;
}

SrvRemoveObject SrvRemoveObject::create(const uint16_t& id) {
    SrvRemoveObject packet;
    packet.set_id(id);
    return packet;
}

SrvRemoveObject SrvRemoveObject::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return SrvRemoveObject(reader);
}

std::unique_ptr<SrvRemoveObject> SrvRemoveObject::allocate(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return std::make_unique<SrvRemoveObject>(reader);
}

void SrvRemoveObject::pack(CRoseBasePolicy& writer) const {
    if (!writer.set_uint16_t(id)) {
        return;
    }
}

constexpr size_t SrvRemoveObject::size() {
    size_t size = 0;
    size += sizeof(uint16_t); // id
    return size;
}


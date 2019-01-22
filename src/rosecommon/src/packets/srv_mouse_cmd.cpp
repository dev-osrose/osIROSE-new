#include "srv_mouse_cmd.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;



SrvMouseCmd::SrvMouseCmd() : CRosePacket(SrvMouseCmd::PACKET_ID) {}

SrvMouseCmd::SrvMouseCmd(CRoseReader reader) : CRosePacket(reader) {
    if (!reader.get_uint16_t(id)) {
        return;
    }
    if (!reader.get_uint16_t(targetId)) {
        return;
    }
    if (!reader.get_uint16_t(distance)) {
        return;
    }
    if (!reader.get_float(x)) {
        return;
    }
    if (!reader.get_float(y)) {
        return;
    }
    if (!reader.get_uint16_t(z)) {
        return;
    }
}

void SrvMouseCmd::set_id(const uint16_t id) {
    this->id = id;
}

uint16_t SrvMouseCmd::get_id() const {
    return id;
}

void SrvMouseCmd::set_targetId(const uint16_t targetId) {
    this->targetId = targetId;
}

uint16_t SrvMouseCmd::get_targetId() const {
    return targetId;
}

void SrvMouseCmd::set_distance(const uint16_t distance) {
    this->distance = distance;
}

uint16_t SrvMouseCmd::get_distance() const {
    return distance;
}

void SrvMouseCmd::set_x(const float x) {
    this->x = x;
}

float SrvMouseCmd::get_x() const {
    return x;
}

void SrvMouseCmd::set_y(const float y) {
    this->y = y;
}

float SrvMouseCmd::get_y() const {
    return y;
}

void SrvMouseCmd::set_z(const uint16_t z) {
    this->z = z;
}

uint16_t SrvMouseCmd::get_z() const {
    return z;
}

SrvMouseCmd SrvMouseCmd::create(const uint16_t& id) {
    SrvMouseCmd packet;
    packet.set_id(id);
    return packet;
}

SrvMouseCmd SrvMouseCmd::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return SrvMouseCmd(reader);
}

std::unique_ptr<SrvMouseCmd> SrvMouseCmd::allocate(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return std::make_unique<SrvMouseCmd>(reader);
}

void SrvMouseCmd::pack(CRoseBasePolicy& writer) const {
    if (!writer.set_uint16_t(id)) {
        return;
    }
    if (!writer.set_uint16_t(targetId)) {
        return;
    }
    if (!writer.set_uint16_t(distance)) {
        return;
    }
    if (!writer.set_float(x)) {
        return;
    }
    if (!writer.set_float(y)) {
        return;
    }
    if (!writer.set_uint16_t(z)) {
        return;
    }
}

constexpr size_t SrvMouseCmd::size() {
    size_t size = 0;
    size += sizeof(uint16_t); // id
    size += sizeof(uint16_t); // targetId
    size += sizeof(uint16_t); // distance
    size += sizeof(float); // x
    size += sizeof(float); // y
    size += sizeof(uint16_t); // z
    return size;
}


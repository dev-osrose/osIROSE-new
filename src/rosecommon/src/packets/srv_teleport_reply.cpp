#include "srv_teleport_reply.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;



SrvTeleportReply::SrvTeleportReply() : CRosePacket(SrvTeleportReply::PACKET_ID) {}

SrvTeleportReply::SrvTeleportReply(CRoseReader reader) : CRosePacket(reader) {
    if (!reader.get_uint16_t(id)) {
        return;
    }
    if (!reader.get_uint16_t(map)) {
        return;
    }
    if (!reader.get_float(x)) {
        return;
    }
    if (!reader.get_float(y)) {
        return;
    }
    if (!reader.get_uint8_t(moveMode)) {
        return;
    }
    if (!reader.get_uint8_t(rideMode)) {
        return;
    }
}

void SrvTeleportReply::set_id(const uint16_t id) {
    this->id = id;
}

uint16_t SrvTeleportReply::get_id() const {
    return id;
}

void SrvTeleportReply::set_map(const uint16_t map) {
    this->map = map;
}

uint16_t SrvTeleportReply::get_map() const {
    return map;
}

void SrvTeleportReply::set_x(const float x) {
    this->x = x;
}

float SrvTeleportReply::get_x() const {
    return x;
}

void SrvTeleportReply::set_y(const float y) {
    this->y = y;
}

float SrvTeleportReply::get_y() const {
    return y;
}

void SrvTeleportReply::set_moveMode(const uint8_t moveMode) {
    this->moveMode = moveMode;
}

uint8_t SrvTeleportReply::get_moveMode() const {
    return moveMode;
}

void SrvTeleportReply::set_rideMode(const uint8_t rideMode) {
    this->rideMode = rideMode;
}

uint8_t SrvTeleportReply::get_rideMode() const {
    return rideMode;
}

SrvTeleportReply SrvTeleportReply::create(const uint16_t& id, const uint16_t& map, const float& x, const float& y, const uint8_t& moveMode, const uint8_t& rideMode) {
    SrvTeleportReply packet;
    packet.set_id(id);
    packet.set_map(map);
    packet.set_x(x);
    packet.set_y(y);
    packet.set_moveMode(moveMode);
    packet.set_rideMode(rideMode);
    return packet;
}

SrvTeleportReply SrvTeleportReply::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return SrvTeleportReply(reader);
}

std::unique_ptr<SrvTeleportReply> SrvTeleportReply::allocate(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return std::make_unique<SrvTeleportReply>(reader);
}

bool SrvTeleportReply::pack(CRoseBasePolicy& writer) const {
    if (!writer.set_uint16_t(id)) {
        return false;
    }
    if (!writer.set_uint16_t(map)) {
        return false;
    }
    if (!writer.set_float(x)) {
        return false;
    }
    if (!writer.set_float(y)) {
        return false;
    }
    if (!writer.set_uint8_t(moveMode)) {
        return false;
    }
    if (!writer.set_uint8_t(rideMode)) {
        return false;
    }
    return true;
}

constexpr size_t SrvTeleportReply::size() {
    size_t size = 0;
    size += sizeof(uint16_t); // id
    size += sizeof(uint16_t); // map
    size += sizeof(float); // x
    size += sizeof(float); // y
    size += sizeof(uint8_t); // moveMode
    size += sizeof(uint8_t); // rideMode
    return size;
}


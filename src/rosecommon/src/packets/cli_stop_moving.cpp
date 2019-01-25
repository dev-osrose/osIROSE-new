#include "cli_stop_moving.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;



CliStopMoving::CliStopMoving() : CRosePacket(CliStopMoving::PACKET_ID) {}

CliStopMoving::CliStopMoving(CRoseReader reader) : CRosePacket(reader) {
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

void CliStopMoving::set_x(const float x) {
    this->x = x;
}

float CliStopMoving::get_x() const {
    return x;
}

void CliStopMoving::set_y(const float y) {
    this->y = y;
}

float CliStopMoving::get_y() const {
    return y;
}

void CliStopMoving::set_z(const uint16_t z) {
    this->z = z;
}

uint16_t CliStopMoving::get_z() const {
    return z;
}

CliStopMoving CliStopMoving::create(const float& x, const float& y, const uint16_t& z) {
    CliStopMoving packet;
    packet.set_x(x);
    packet.set_y(y);
    packet.set_z(z);
    return packet;
}

CliStopMoving CliStopMoving::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return CliStopMoving(reader);
}

std::unique_ptr<CliStopMoving> CliStopMoving::allocate(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return std::make_unique<CliStopMoving>(reader);
}

bool CliStopMoving::pack(CRoseBasePolicy& writer) const {
    if (!writer.set_float(x)) {
        return false;
    }
    if (!writer.set_float(y)) {
        return false;
    }
    if (!writer.set_uint16_t(z)) {
        return false;
    }
    return true;
}

constexpr size_t CliStopMoving::size() {
    size_t size = 0;
    size += sizeof(float); // x
    size += sizeof(float); // y
    size += sizeof(uint16_t); // z
    return size;
}


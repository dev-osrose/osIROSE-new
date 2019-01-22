#include "cli_mouse_cmd.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;



CliMouseCmd::CliMouseCmd() : CRosePacket(CliMouseCmd::PACKET_ID) {}

CliMouseCmd::CliMouseCmd(CRoseReader reader) : CRosePacket(reader) {
    if (!reader.get_uint16_t(targetId)) {
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

void CliMouseCmd::set_targetId(const uint16_t targetId) {
    this->targetId = targetId;
}

uint16_t CliMouseCmd::get_targetId() const {
    return targetId;
}

void CliMouseCmd::set_x(const float x) {
    this->x = x;
}

float CliMouseCmd::get_x() const {
    return x;
}

void CliMouseCmd::set_y(const float y) {
    this->y = y;
}

float CliMouseCmd::get_y() const {
    return y;
}

void CliMouseCmd::set_z(const uint16_t z) {
    this->z = z;
}

uint16_t CliMouseCmd::get_z() const {
    return z;
}

CliMouseCmd CliMouseCmd::create(const uint16_t& targetId, const float& x, const float& y, const uint16_t& z) {
    CliMouseCmd packet;
    packet.set_targetId(targetId);
    packet.set_x(x);
    packet.set_y(y);
    packet.set_z(z);
    return packet;
}

CliMouseCmd CliMouseCmd::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return CliMouseCmd(reader);
}

std::unique_ptr<CliMouseCmd> CliMouseCmd::allocate(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return std::make_unique<CliMouseCmd>(reader);
}

bool CliMouseCmd::pack(CRoseBasePolicy& writer) const {
    if (!writer.set_uint16_t(targetId)) {
        return false;
    }
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

constexpr size_t CliMouseCmd::size() {
    size_t size = 0;
    size += sizeof(uint16_t); // targetId
    size += sizeof(float); // x
    size += sizeof(float); // y
    size += sizeof(uint16_t); // z
    return size;
}


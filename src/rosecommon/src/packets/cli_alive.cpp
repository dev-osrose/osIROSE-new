#include "cli_alive.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;



CliAlive::CliAlive() : CRosePacket(CliAlive::PACKET_ID) {}

CliAlive::CliAlive(CRoseReader reader) : CRosePacket(reader) {
}

CliAlive CliAlive::create() {
    CliAlive packet;
    return packet;
}

CliAlive CliAlive::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return CliAlive(reader);
}

std::unique_ptr<CliAlive> CliAlive::allocate(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return std::make_unique<CliAlive>(reader);
}

bool CliAlive::pack(CRoseBasePolicy&) const {
    return true;
}

constexpr size_t CliAlive::size() {
    size_t size = 0;
    return size;
}


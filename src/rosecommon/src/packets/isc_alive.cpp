#include "isc_alive.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;



IscAlive::IscAlive() : CRosePacket(IscAlive::PACKET_ID) {}

IscAlive::IscAlive(CRoseReader reader) : CRosePacket(reader) {
}

IscAlive IscAlive::create() {
    IscAlive packet;
    return packet;
}

IscAlive IscAlive::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return IscAlive(reader);
}

std::unique_ptr<IscAlive> IscAlive::allocate(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return std::make_unique<IscAlive>(reader);
}

bool IscAlive::pack(CRoseBasePolicy&) const {
    return true;
}

constexpr size_t IscAlive::size() {
    size_t size = 0;
    return size;
}


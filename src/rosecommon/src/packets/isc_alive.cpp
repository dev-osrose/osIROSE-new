#include "isc_alive.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;



IscAlive::IscAlive() : CRosePacket(ePacketType::ISC_ALIVE) {}

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

void IscAlive::pack(CRoseBasePolicy&) const {
}

constexpr size_t IscAlive::size() {
    size_t size = 0;
    return size;
}


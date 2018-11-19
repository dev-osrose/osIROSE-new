#include "isc_alive.h"
#include "throwassert.h"

namespace RoseCommon {

IscAlive::IscAlive() : CRosePacket(ePacketType::ISC_ALIVE) {}

IscAlive::IscAlive(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::ISC_ALIVE, "Not the right packet: " << to_underlying(get_type()));
}


void IscAlive::pack(CRoseBasePolicy&) const {
}

IscAlive IscAlive::create() {


	return IscAlive();
}

IscAlive IscAlive::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return IscAlive(reader);
}
std::unique_ptr<IscAlive> IscAlive::allocate(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return std::make_unique<IscAlive>(reader);
}

}
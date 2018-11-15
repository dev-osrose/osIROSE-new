#include "isc_alive.h"
#include "throwassert.h"

namespace RoseCommon {

IscAlive::IscAlive() : CRosePacket(ePacketType::ISC_ALIVE) {}

IscAlive::IscAlive(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::ISC_ALIVE, "Not the right packet: " << to_underlying(get_type()));
}


void IscAlive::pack(CRoseWriter&) const {
}

uint16_t IscAlive::get_size() const {
	uint16_t size = 0;
	return size;
}


IscAlive IscAlive::create() {


	return IscAlive();
}

IscAlive IscAlive::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return IscAlive(reader);
}

}
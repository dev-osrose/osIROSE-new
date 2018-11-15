#include "cli_alive.h"
#include "throwassert.h"

namespace RoseCommon {

CliAlive::CliAlive() : CRosePacket(ePacketType::PAKCS_ALIVE) {}

CliAlive::CliAlive(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKCS_ALIVE, "Not the right packet: " << to_underlying(get_type()));
}


void CliAlive::pack(CRoseWriter&) const {
}

uint16_t CliAlive::get_size() const {
	uint16_t size = 0;
	return size;
}


CliAlive CliAlive::create() {


	return CliAlive();
}

CliAlive CliAlive::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return CliAlive(reader);
}

}
#include "cli_alive.h"
#include "throwassert.h"

namespace RoseCommon {

CliAlive::CliAlive() : CRosePacket(ePacketType::PAKCS_ALIVE) {}

CliAlive::CliAlive(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	throw_assert(type() != ePacketType::PAKCS_ALIVE, "Not the right packet: " << to_underlying(type()));
}


void CliAlive::pack() {
}

}

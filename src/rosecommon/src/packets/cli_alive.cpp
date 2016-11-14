#include "cli_alive.h"

namespace RoseCommon {

CliAlive::CliAlive(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	if (type() != ePacketType::PAKCS_ALIVE)
		throw std::runtime_error("Not the right packet!");

}

CliAlive::CliAlive() : CRosePacket(ePacketType::PAKCS_ALIVE) {}

}

#include "cli_stop.h"

namespace RoseCommon {

CliStop::CliStop(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	if (type() != ePacketType::PAKCS_STOP)
		throw std::runtime_error("Not the right packet!");
	*this >> x_;
	*this >> y_;

}

float &CliStop::x() {
	return x_;
}

float &CliStop::y() {
	return y_;
}

}
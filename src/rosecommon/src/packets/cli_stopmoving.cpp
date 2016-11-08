#include "cli_stopmoving.h"

namespace RoseCommon {

CliStopMoving::CliStopMoving(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	if (type() != ePacketType::PAKCS_STOP_MOVING)
		throw std::runtime_error("Not the right packet!");
	*this >> x_;
	*this >> y_;
	*this >> z_;

}

float &CliStopMoving::x() {
	return x_;
}

float &CliStopMoving::y() {
	return y_;
}

int16_t &CliStopMoving::z() {
	return z_;
}

}
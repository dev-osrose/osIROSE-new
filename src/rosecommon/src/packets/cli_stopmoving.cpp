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

const float &CliStopMoving::x() const {
    return x_;
}

float &CliStopMoving::y() {
	return y_;
}

const float &CliStopMoving::y() const {
    return y_;
}

int16_t &CliStopMoving::z() {
	return z_;
}

const int16_t &CliStopMoving::z() const {
    return z_;
}

}

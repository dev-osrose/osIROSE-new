#include "cli_mousecmd.h"

namespace RoseCommon {

CliMouseCmd::CliMouseCmd(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	if (type() != ePacketType::PAKCS_MOUSE_CMD)
		throw std::runtime_error("Not the right packet!");
	*this >> targetId_ >> x_ >> y_ >> z_;
}

uint16_t &CliMouseCmd::targetId() {
	return targetId_;
}

const uint16_t &CliMouseCmd::targetId() const {
    return targetId_;
}

float &CliMouseCmd::x() {
	return x_;
}

const float &CliMouseCmd::x() const {
    return x_;
}

float &CliMouseCmd::y() {
	return y_;
}

const float &CliMouseCmd::y() const {
    return y_;
}

uint16_t &CliMouseCmd::z() {
	return z_;
}

const uint16_t &CliMouseCmd::z() const {
    return z_;
}

}

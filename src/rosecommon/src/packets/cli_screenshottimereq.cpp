#include "cli_screenshottimereq.h"

namespace RoseCommon {

CliScreenShotTimeReq::CliScreenShotTimeReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	if (type() != ePacketType::PAKCS_SCREEN_SHOT_TIME_REQ)
		throw std::runtime_error("Not the right packet!");
	*this >> count_;

}

uint16_t &CliScreenShotTimeReq::count() {
	return count_;
}

}
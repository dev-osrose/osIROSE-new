#include "cli_changemapreq.h"

namespace RoseCommon {

CliChangeMapReq::CliChangeMapReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	if (type() != ePacketType::PAKCS_CHANGE_MAP_REQ)
		throw std::runtime_error("Not the right packet!");
	*this >> weightRate_;
	*this >> z_;

}

uint8_t &CliChangeMapReq::weightRate() {
	return weightRate_;
}

uint16_t &CliChangeMapReq::z() {
	return z_;
}

}
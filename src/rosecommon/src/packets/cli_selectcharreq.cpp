#include "cli_selectcharreq.h"

namespace RoseCommon {

CliSelectCharReq::CliSelectCharReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	if (type() != ePacketType::PAKCS_SELECT_CHAR_REQ)
		throw std::runtime_error("Not the right packet!");
	*this >> charId_;
	*this >> runMode_;
	*this >> rideMode_;
	*this >> name_;

}

uint8_t &CliSelectCharReq::charId() {
	return charId_;
}

uint8_t &CliSelectCharReq::runMode() {
	return runMode_;
}

uint8_t &CliSelectCharReq::rideMode() {
	return rideMode_;
}

std::string &CliSelectCharReq::name() {
	return name_;
}

}
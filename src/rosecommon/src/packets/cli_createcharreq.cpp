#include "cli_createcharreq.h"

namespace RoseCommon {

CliCreateCharReq::CliCreateCharReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	if (type() != ePacketType::PAKCS_CREATE_CHAR_REQ)
		throw std::runtime_error("Not the right packet!");
	*this >> race_;
	*this >> stone_;
	*this >> hair_;
	*this >> face_;
	*this >> weapon_;
	*this >> zone_;
	*this >> name_;

}

CliCreateCharReq::CliCreateCharReq(uint8_t race, uint8_t stone, uint8_t hair, uint8_t face, uint8_t weapon, uint8_t zone, const std::string &name) :
    CRosePacket(ePacketType::PAKCS_CREATE_CHAR_REQ), race_(race), stone_(stone), hair_(hair), face_(face), weapon_(weapon), zone_(zone), name_(name) {}

uint8_t &CliCreateCharReq::race() {
	return race_;
}

uint8_t &CliCreateCharReq::stone() {
	return stone_;
}

uint8_t &CliCreateCharReq::hair() {
	return hair_;
}

uint8_t &CliCreateCharReq::face() {
	return face_;
}

uint8_t &CliCreateCharReq::weapon() {
	return weapon_;
}

uint16_t &CliCreateCharReq::zone() {
	return zone_;
}

std::string &CliCreateCharReq::name() {
	return name_;
}

}

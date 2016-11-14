#include "cli_loginreq.h"

namespace RoseCommon {

CliLoginReq::CliLoginReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	if (type() != ePacketType::PAKCS_LOGIN_REQ)
		throw std::runtime_error("Not the right packet!");
	*this >> username_;
	*this >> password_;

}

CliLoginReq::CliLoginReq(const std::string &user, const std::string &password) : CRosePacket(ePacketType::PAKCS_LOGIN_REQ), username_(user), password_(password) {}

std::string &CliLoginReq::username() {
	return username_;
}

std::string &CliLoginReq::password() {
	return password_;
}

}

#include "cli_loginreq.h"

namespace RoseCommon {

CliLoginReq::CliLoginReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	if (type() != ePacketType::PAKCS_LOGIN_REQ)
		throw std::runtime_error("Not the right packet!");
    char pass[32];
	*this >> pass;
	*this >> username_;
    password_ = std::string(pass, 32);
}

CliLoginReq::CliLoginReq(const std::string &user, const std::string &password) : CRosePacket(ePacketType::PAKCS_LOGIN_REQ), username_(user), password_(password) {}

void CliLoginReq::pack() {
    *this << password_.c_str() << username_;
}

std::string &CliLoginReq::username() {
	return username_;
}

std::string &CliLoginReq::password() {
	return password_;
}

}

#include "cli_loginreq.h"

namespace RoseCommon {

CliLoginReq::CliLoginReq() : CRosePacket(ePacketType::PAKCS_LOGIN_REQ) {}

CliLoginReq::CliLoginReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	if (type() != ePacketType::PAKCS_LOGIN_REQ)
		throw std::runtime_error("Not the right packet!");
    char pass[32];
    *this >> pass;
	*this >> username_;
    password_ = std::string(pass, 32);
}

CliLoginReq::CliLoginReq(const std::string &password, const std::string &username) : CRosePacket(ePacketType::PAKCS_LOGIN_REQ), password_(password), username_(username) {}

std::string &CliLoginReq::password() {
	return password_;
}

const std::string &CliLoginReq::password() const {
	return password_;
}

std::string &CliLoginReq::username() {
	return username_;
}

const std::string &CliLoginReq::username() const {
	return username_;
}


void CliLoginReq::pack() {
    *this << password_.c_str();
	*this << username_;
}

}

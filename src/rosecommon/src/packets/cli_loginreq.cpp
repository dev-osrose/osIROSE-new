#include "cli_loginreq.h"
#include "throwassert.h"

namespace RoseCommon {

const RecvPacketFactory::Initializer<uint8_t[MAX_PACKET_SIZE]> CliLoginReq::init = RecvPacketFactory::Initializer<uint8_t[MAX_PACKET_SIZE]>(ePacketType::PAKCS_LOGIN_REQ, &createPacket<CliLoginReq>);

CliLoginReq::CliLoginReq() : CRosePacket(ePacketType::PAKCS_LOGIN_REQ) {}

CliLoginReq::CliLoginReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	throw_assert(type() == ePacketType::PAKCS_LOGIN_REQ, "Not the right packet: " << to_underlying(type()));
    char pass[32];
    *this >> pass;
    password_ = std::string(pass, 32);
	*this >> username_;
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

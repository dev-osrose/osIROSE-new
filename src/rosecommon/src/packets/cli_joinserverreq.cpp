#include "cli_joinserverreq.h"
#include "throwassert.h"

namespace RoseCommon {

CliJoinServerReq::CliJoinServerReq() : CRosePacket(ePacketType::PAKCS_JOIN_SERVER_REQ) {}

CliJoinServerReq::CliJoinServerReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	throw_assert(type() != ePacketType::PAKCS_JOIN_SERVER_REQ, "Not the right packet: " << to_underlying(type()));
    *this >> sessionId_;
    char pass[32];
    *this >> pass;
    password_ = std::string(pass, 32);
}

CliJoinServerReq::CliJoinServerReq(uint32_t sessionId, const std::string &password) : CRosePacket(ePacketType::PAKCS_JOIN_SERVER_REQ), sessionId_(sessionId), password_(password) {}

uint32_t CliJoinServerReq::sessionId() const {
	return sessionId_;
}

std::string &CliJoinServerReq::password() {
	return password_;
}

const std::string &CliJoinServerReq::password() const {
	return password_;
}


void CliJoinServerReq::pack() {
	*this << sessionId_;
	*this << password_.c_str();
}

}

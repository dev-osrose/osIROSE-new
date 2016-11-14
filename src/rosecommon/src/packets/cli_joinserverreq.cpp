#include "cli_joinserverreq.h"

namespace RoseCommon {

CliJoinServerReq::CliJoinServerReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	if (type() != ePacketType::PAKCS_JOIN_SERVER_REQ)
		throw std::runtime_error("Not the right packet!");
	*this >> sessionId_;
	*this >> password_;

}

CliJoinServerReq::CliJoinServerReq(uint32_t sessionId, const std::string& password) : CRosePacket(ePacketType::PAKCS_JOIN_SERVER_REQ), sessionId_(sessionId), password_(password) {}

uint32_t &CliJoinServerReq::sessionId() {
	return sessionId_;
}

std::string &CliJoinServerReq::password() {
	return password_;
}

}

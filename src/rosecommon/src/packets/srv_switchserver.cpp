#include "srv_switchserver.h"
#include "throwassert.h"

namespace RoseCommon {

SrvSwitchServer::SrvSwitchServer() : CRosePacket(ePacketType::PAKCC_SWITCH_SERVER) {}

SrvSwitchServer::SrvSwitchServer(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	throw_assert(type() == ePacketType::PAKCC_SWITCH_SERVER, "Not the right packet: " << to_underlying(type()));
	*this >> port_;
	*this >> sessionId_;
	*this >> sessionSeed_;
	*this >> ip_;
}

SrvSwitchServer::SrvSwitchServer(uint16_t port, uint32_t sessionId, uint32_t sessionSeed, const std::string &ip) : CRosePacket(ePacketType::PAKCC_SWITCH_SERVER), port_(port), sessionId_(sessionId), sessionSeed_(sessionSeed), ip_(ip) {}

uint16_t SrvSwitchServer::port() const {
	return port_;
}

uint32_t SrvSwitchServer::sessionId() const {
	return sessionId_;
}

uint32_t SrvSwitchServer::sessionSeed() const {
	return sessionSeed_;
}

std::string &SrvSwitchServer::ip() {
	return ip_;
}

const std::string &SrvSwitchServer::ip() const {
	return ip_;
}


void SrvSwitchServer::pack() {
	*this << port_;
	*this << sessionId_;
	*this << sessionSeed_;
	*this << ip_;
}

}

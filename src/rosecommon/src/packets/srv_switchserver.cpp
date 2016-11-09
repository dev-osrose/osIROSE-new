#include "srv_switchserver.h"

namespace RoseCommon {

SrvSwitchServer::SrvSwitchServer() : CRosePacket(ePacketType::PAKCC_SWITCH_SERVER) {
}

SrvSwitchServer::SrvSwitchServer(uint16_t port, uint32_t sessionId, uint32_t sessionSeed, std::string ip) : CRosePacket(ePacketType::PAKCC_SWITCH_SERVER), port_(port), sessionId_(sessionId), sessionSeed_(sessionSeed), ip_(ip) {
}

uint16_t &SrvSwitchServer::port() {
	return port_;
}

uint32_t &SrvSwitchServer::sessionId() {
	return sessionId_;
}

uint32_t &SrvSwitchServer::sessionSeed() {
	return sessionSeed_;
}

std::string &SrvSwitchServer::ip() {
	return ip_;
}

void SrvSwitchServer::pack() {
	*this << port_;
	*this << sessionId_;
	*this << sessionSeed_;
	*this << ip_;
}

}
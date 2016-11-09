#include "srv_srvselectreply.h"

namespace RoseCommon {

SrvSrvSelectReply::SrvSrvSelectReply() : CRosePacket(ePacketType::PAKLC_SRV_SELECT_REPLY) {
}

SrvSrvSelectReply::SrvSrvSelectReply(uint32_t sessionId, uint32_t cryptVal, uint16_t port, eResult result, std::string ip) : CRosePacket(ePacketType::PAKLC_SRV_SELECT_REPLY), sessionId_(sessionId), cryptVal_(cryptVal), port_(port), result_(result), ip_(ip) {
}

uint32_t &SrvSrvSelectReply::sessionId() {
	return sessionId_;
}

uint32_t &SrvSrvSelectReply::cryptVal() {
	return cryptVal_;
}

uint16_t &SrvSrvSelectReply::port() {
	return port_;
}

SrvSrvSelectReply::eResult &SrvSrvSelectReply::result() {
	return result_;
}

std::string &SrvSrvSelectReply::ip() {
	return ip_;
}

void SrvSrvSelectReply::pack() {
	*this << sessionId_;
	*this << cryptVal_;
	*this << port_;
	*this << result_;
	*this << ip_;
}

}

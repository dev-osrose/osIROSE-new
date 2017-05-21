#include "srv_srvselectreply.h"

namespace RoseCommon {

SrvSrvSelectReply::SrvSrvSelectReply() : CRosePacket(ePacketType::PAKLC_SRV_SELECT_REPLY) {}

SrvSrvSelectReply::SrvSrvSelectReply(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    throw_assert(type() == ePacketType::PAKLC_SRV_SELECT_REPLY, "Not the right packet: " << to_underlying(type()));
    *this >> result_;
    *this >> sessionId_;
    *this >> cryptVal_;
    *this >> ip_;
    *this >> port_;
}

SrvSrvSelectReply::SrvSrvSelectReply(SrvSrvSelectReply::eResult result, uint32_t sessionId, uint32_t cryptVal, const std::string &ip, uint16_t port) : CRosePacket(ePacketType::PAKLC_SRV_SELECT_REPLY), result_(result), sessionId_(sessionId), cryptVal_(cryptVal), ip_(ip), port_(port) {}

SrvSrvSelectReply::eResult SrvSrvSelectReply::result() const {
	return result_;
}

uint32_t SrvSrvSelectReply::sessionId() const {
	return sessionId_;
}

uint32_t SrvSrvSelectReply::cryptVal() const {
	return cryptVal_;
}

std::string &SrvSrvSelectReply::ip() {
	return ip_;
}

const std::string &SrvSrvSelectReply::ip() const {
	return ip_;
}

uint16_t SrvSrvSelectReply::port() const {
	return port_;
}


void SrvSrvSelectReply::pack() {
    *this << result_;
	*this << sessionId_;
	*this << cryptVal_;
	*this << ip_;
	*this << port_;
}

}

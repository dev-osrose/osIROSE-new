#include "cli_srvselectreq.h"

namespace RoseCommon {

CliSrvSelectReq::CliSrvSelectReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	if (type() != ePacketType::PAKCS_SRV_SELECT_REQ)
		throw std::runtime_error("Not the right packet!");
	*this >> serverId_;
	*this >> channelId_;

}

CliSrvSelectReq::CliSrvSelectReq(uint32_t serverId, uint8_t channelId) : CRosePacket(ePacketType::PAKCS_SRV_SELECT_REQ), serverId_(serverId), channelId_(channelId) {}

void CliSrvSelectReq::pack() {
    *this << serverId_ << channelId_;
}

uint32_t &CliSrvSelectReq::serverId() {
	return serverId_;
}

uint8_t &CliSrvSelectReq::channelId() {
	return channelId_;
}

}

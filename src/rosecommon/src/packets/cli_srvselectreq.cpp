#include "cli_srvselectreq.h"

namespace RoseCommon {

CliSrvSelectReq::CliSrvSelectReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	if (type() != ePacketType::PAKCS_SRV_SELECT_REQ)
		throw std::runtime_error("Not the right packet!");
	*this >> serverId_;
	*this >> channelId_;

}

uint32_t &CliSrvSelectReq::serverId() {
	return serverId_;
}

uint8_t &CliSrvSelectReq::channelId() {
	return channelId_;
}

}
#include "cli_channellistreq.h"

namespace RoseCommon {

CliChannelListReq::CliChannelListReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	if (type() != ePacketType::PAKCS_CHANNEL_LIST_REQ)
		throw std::runtime_error("Not the right packet!");
	*this >> serverId_;

}

uint32_t &CliChannelListReq::serverId() {
	return serverId_;
}

}
#include "cli_channellistreq.h"
#include "throwassert.h"

namespace RoseCommon {

const RecvPacketFactory::Initializer<uint8_t[MAX_PACKET_SIZE]> CliChannelListReq::init = RecvPacketFactory::Initializer<uint8_t[MAX_PACKET_SIZE]>(ePacketType::PAKCS_CHANNEL_LIST_REQ, &createPacket<CliChannelListReq>);

CliChannelListReq::CliChannelListReq() : CRosePacket(ePacketType::PAKCS_CHANNEL_LIST_REQ) {}

CliChannelListReq::CliChannelListReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	throw_assert(type() == ePacketType::PAKCS_CHANNEL_LIST_REQ, "Not the right packet: " << to_underlying(type()));
	*this >> serverId_;
}

CliChannelListReq::CliChannelListReq(uint32_t serverId) : CRosePacket(ePacketType::PAKCS_CHANNEL_LIST_REQ), serverId_(serverId) {}

uint32_t CliChannelListReq::serverId() const {
	return serverId_;
}


void CliChannelListReq::pack() {
	*this << serverId_;
}

}
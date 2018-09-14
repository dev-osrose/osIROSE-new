#include "cli_channellistreq.h"
#include "throwassert.h"

namespace RoseCommon {

const RecvPacketFactory::Initializer<uint8_t*> CliChannelListReq::init = RecvPacketFactory::Initializer<uint8_t*>(ePacketType::PAKCS_CHANNEL_LIST_REQ, &createPacket<CliChannelListReq>);

CliChannelListReq::CliChannelListReq() : CRosePacket(ePacketType::PAKCS_CHANNEL_LIST_REQ) {}

CliChannelListReq::CliChannelListReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	throw_assert(type() == ePacketType::PAKCS_CHANNEL_LIST_REQ, "Not the right packet: " << to_underlying(type()));
	serverId_ = read_uint32();
}

CliChannelListReq::CliChannelListReq(uint32_t serverId) : CRosePacket(ePacketType::PAKCS_CHANNEL_LIST_REQ), serverId_(serverId) {}

uint32_t CliChannelListReq::serverId() const {
	return serverId_;
}

void CliChangeChannelListReq::serverId(uint32_t id) {
	serverId_ = id;
}

void CliChannelListReq::pack() {
	write_uint32(serverId_);
}

}

#include "cli_channellistreq.h"
#include "throwassert.h"

namespace RoseCommon {

CliChannelListReq::CliChannelListReq() : CRosePacket(ePacketType::PAKCS_CHANNEL_LIST_REQ) {}

CliChannelListReq::CliChannelListReq(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKCS_CHANNEL_LIST_REQ, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint32_t(serverId_);
}

CliChannelListReq::CliChannelListReq(uint32_t serverId) : CRosePacket(ePacketType::PAKCS_CHANNEL_LIST_REQ), serverId_(serverId) {
}

uint32_t CliChannelListReq::serverId() const { return serverId_; }

CliChannelListReq& CliChannelListReq::set_serverId(uint32_t data) { serverId_ = data; return *this; }


void CliChannelListReq::pack(CRoseWriter& writer) const {
	writer.set_uint32_t(serverId_);
}

uint16_t CliChannelListReq::get_size() const {
	uint16_t size = 0;
	size += sizeof(serverId_);
	return size;
}


CliChannelListReq CliChannelListReq::create(uint32_t serverId) {


	return CliChannelListReq(serverId);
}

CliChannelListReq CliChannelListReq::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return CliChannelListReq(reader);
}

}
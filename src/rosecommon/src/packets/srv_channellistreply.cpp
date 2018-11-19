#include "srv_channellistreply.h"
#include "throwassert.h"

namespace RoseCommon {

SrvChannelListReply::SrvChannelListReply() : CRosePacket(ePacketType::PAKLC_CHANNEL_LIST_REPLY) {}

SrvChannelListReply::SrvChannelListReply(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKLC_CHANNEL_LIST_REPLY, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint32_t(id_);
	{
		uint8_t index = 0;
		reader.get_uint8_t(index);
		for (uint8_t i = 0; i < index; ++i) {
			ChannelListReply::ChannelInfo tmp;
			reader.get_iserialize(tmp);
			channels_.push_back(tmp);
		}
	}
}

SrvChannelListReply::SrvChannelListReply(uint32_t id) : CRosePacket(ePacketType::PAKLC_CHANNEL_LIST_REPLY), id_(id) {
}

uint32_t SrvChannelListReply::id() const { return id_; }

SrvChannelListReply& SrvChannelListReply::set_id(uint32_t data) { id_ = data; return *this; }

const std::vector<ChannelListReply::ChannelInfo>& SrvChannelListReply::channels() const { return channels_; }

SrvChannelListReply& SrvChannelListReply::set_channels(const std::vector<ChannelListReply::ChannelInfo>& data) { channels_ = data; return *this; }

SrvChannelListReply& SrvChannelListReply::add_channelinfo(const ChannelListReply::ChannelInfo& data) { channels_.push_back(data); return *this; }


void SrvChannelListReply::pack(CRoseBasePolicy& writer) const {
	writer.set_uint32_t(id_);
	writer.set_uint8_t(channels_.size());
	for (const auto& elem : channels_) writer.set_iserialize(elem);
}

SrvChannelListReply SrvChannelListReply::create(uint32_t id) {


	return SrvChannelListReply(id);
}

SrvChannelListReply SrvChannelListReply::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvChannelListReply(reader);
}
std::unique_ptr<SrvChannelListReply> SrvChannelListReply::allocate(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return std::make_unique<SrvChannelListReply>(reader);
}

}
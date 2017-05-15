#include "srv_channellistreply.h"
#include "throwassert.h"

namespace RoseCommon {

SrvChannelListReply::SrvChannelListReply() : CRosePacket(ePacketType::PAKLC_CHANNEL_LIST_REPLY) {}

SrvChannelListReply::SrvChannelListReply(uint32_t id) : CRosePacket(ePacketType::PAKLC_CHANNEL_LIST_REPLY), id_(id) {}

SrvChannelListReply::SrvChannelListReply(uint32_t id, const std::vector<SrvChannelListReply::ChannelInfo> &channels) : CRosePacket(ePacketType::PAKLC_CHANNEL_LIST_REPLY), id_(id), channels_(channels) {}

uint32_t SrvChannelListReply::id() const {
	return id_;
}

std::vector<SrvChannelListReply::ChannelInfo> &SrvChannelListReply::channels() {
	return channels_;
}

const std::vector<SrvChannelListReply::ChannelInfo> &SrvChannelListReply::channels() const {
	return channels_;
}

void SrvChannelListReply::addChannel(const std::string &name, uint8_t id, uint16_t capacity, uint8_t lowAge, uint8_t highAge) {
    channels_.push_back(SrvChannelListReply::ChannelInfo{name, id, lowAge, highAge, capacity});
}

void SrvChannelListReply::pack() {
	*this << id_;
	*this << (uint8_t)channels_.size();
    for (auto &channel : channels_)
        *this << channel.id_
              << channel.lowAge_
              << channel.highAge_
              << channel.capacity_
              << channel.name_;
}

}

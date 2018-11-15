#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <vector>
#include <dataconsts.h>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKLC_CHANNEL_LIST_REPLY, SrvChannelListReply)
REGISTER_SEND_PACKET(ePacketType::PAKLC_CHANNEL_LIST_REPLY, SrvChannelListReply)
class SrvChannelListReply : public CRosePacket {
	public:
		SrvChannelListReply();
		SrvChannelListReply(CRoseReader reader);
	private:
		SrvChannelListReply(uint32_t id);
	public:

		virtual ~SrvChannelListReply() = default;

		uint32_t id() const;
		SrvChannelListReply& set_id(uint32_t);
		const std::vector<ChannelListReply::ChannelInfo>& channels() const;
		SrvChannelListReply& set_channels(const std::vector<ChannelListReply::ChannelInfo>&);
		SrvChannelListReply& add_channelinfo(const ChannelListReply::ChannelInfo&);

		static SrvChannelListReply create(uint32_t id);
		static SrvChannelListReply create(uint8_t *buffer);

	protected:
		virtual void pack(CRoseWriter&) const override;
		virtual uint16_t get_size() const override;

	private:
		uint32_t id_;
		std::vector<ChannelListReply::ChannelInfo> channels_;
};

}
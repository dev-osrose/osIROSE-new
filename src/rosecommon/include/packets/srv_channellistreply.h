#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <vector>
#include <string>

namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKLC_CHANNEL_LIST_REPLY, SrvChannelListReply)
class SrvChannelListReply : public CRosePacket {
	public:
        struct ChannelInfo {
            std::string name_;
            uint8_t id_;
            uint8_t lowAge_;
            uint8_t highAge_;
            uint16_t capacity_;
        };

		SrvChannelListReply();
        SrvChannelListReply(uint8_t buffer[MAX_PACKET_SIZE]);
        SrvChannelListReply(uint32_t id);
		SrvChannelListReply(uint32_t id, const std::vector<ChannelInfo> &channels);

		virtual ~SrvChannelListReply() = default;

		uint32_t id() const;
		std::vector<ChannelInfo> &channels();
		const std::vector<ChannelInfo> &channels() const;

        void addChannel(const std::string &name, uint8_t id, uint16_t capacity, uint8_t lowAge = 0, uint8_t highAge = 0);

	protected:
		virtual void pack() override;

	private:
		uint32_t id_;
		std::vector<ChannelInfo> channels_;
};

}

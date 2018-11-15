#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_CHANNEL_LIST_REQ, CliChannelListReq)
REGISTER_SEND_PACKET(ePacketType::PAKCS_CHANNEL_LIST_REQ, CliChannelListReq)
class CliChannelListReq : public CRosePacket {
	public:
		CliChannelListReq();
		CliChannelListReq(CRoseReader reader);
	private:
		CliChannelListReq(uint32_t serverId);
	public:

		virtual ~CliChannelListReq() = default;

		uint32_t serverId() const;
		CliChannelListReq& set_serverId(uint32_t);

		static CliChannelListReq create(uint32_t serverId);
		static CliChannelListReq create(uint8_t *buffer);

	protected:
		virtual void pack(CRoseWriter&) const override;
		virtual uint16_t get_size() const override;

	private:
		uint32_t serverId_;
};

}
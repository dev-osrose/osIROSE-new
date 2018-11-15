#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_SRV_SELECT_REQ, CliSrvSelectReq)
REGISTER_SEND_PACKET(ePacketType::PAKCS_SRV_SELECT_REQ, CliSrvSelectReq)
class CliSrvSelectReq : public CRosePacket {
	public:
		CliSrvSelectReq();
		CliSrvSelectReq(CRoseReader reader);
	private:
		CliSrvSelectReq(uint32_t serverId, uint8_t channelId);
	public:

		virtual ~CliSrvSelectReq() = default;

		uint32_t serverId() const;
		CliSrvSelectReq& set_serverId(uint32_t);
		uint8_t channelId() const;
		CliSrvSelectReq& set_channelId(uint8_t);

		static CliSrvSelectReq create(uint32_t serverId, uint8_t channelId);
		static CliSrvSelectReq create(uint8_t *buffer);

	protected:
		virtual void pack(CRoseWriter&) const override;
		virtual uint16_t get_size() const override;

	private:
		uint32_t serverId_;
		uint8_t channelId_;
};

}
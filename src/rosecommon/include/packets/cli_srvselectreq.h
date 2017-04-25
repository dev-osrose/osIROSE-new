#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_SRV_SELECT_REQ, CliSrvSelectReq)
class CliSrvSelectReq : public CRosePacket {
	public:
		CliSrvSelectReq();
		CliSrvSelectReq(uint8_t buffer[MAX_PACKET_SIZE]);
		CliSrvSelectReq(uint32_t serverId, uint8_t channelId);

		virtual ~CliSrvSelectReq() = default;

		uint32_t serverId() const;
		uint8_t channelId() const;

	protected:
		virtual void pack() override;

	private:
		uint32_t serverId_;
		uint8_t channelId_;
};

}
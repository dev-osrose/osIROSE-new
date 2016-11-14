#pragma once

#include "packetfactory.h"
#include "entityComponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_SRV_SELECT_REQ, CliSrvSelectReq)
class CliSrvSelectReq : public CRosePacket, public RegisterRecvPacket<ePacketType::PAKCS_SRV_SELECT_REQ, CliSrvSelectReq> {
	public:
		CliSrvSelectReq(uint8_t buffer[MAX_PACKET_SIZE]);
        CliSrvSelectReq(uint32_t serverId, uint8_t channelId);

        virtual void pack() override;

		virtual ~CliSrvSelectReq() = default;

		uint32_t &serverId();
		uint8_t &channelId();

	private:
		uint32_t serverId_;
		uint8_t channelId_;
};

}

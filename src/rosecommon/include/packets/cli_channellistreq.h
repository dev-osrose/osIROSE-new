#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_CHANNEL_LIST_REQ, CliChannelListReq)
class CliChannelListReq : public CRosePacket {
	private:
		static const RecvPacketFactory::Initializer<uint8_t*> init;
	public:
		CliChannelListReq();
		CliChannelListReq(uint8_t buffer[MAX_PACKET_SIZE]);
		CliChannelListReq(uint32_t serverId);

		virtual ~CliChannelListReq() = default;

		uint32_t serverId() const;
		void serverId(uint32_t id);

	protected:
		virtual void pack() override;

	private:
		uint32_t serverId_;
};

}

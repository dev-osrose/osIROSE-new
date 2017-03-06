#pragma once

#include "packetfactory.h"
#include "entityComponents.h"


namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKWC_REVIVE_REPLY, SrvReviveReply)
class SrvReviveReply : public CRosePacket {
	public:
		SrvReviveReply();
		SrvReviveReply(uint16_t mapId);

		virtual ~SrvReviveReply() = default;

		uint16_t mapId() const;

	protected:
		virtual void pack() override;

	private:
		uint16_t mapId_;
};

}
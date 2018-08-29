#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
//-------------------------------------------------------
//TODO: REMOVE THIS FILE AS THIS PACKET ISN'T USED!!!!!!
//-------------------------------------------------------
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
//-------------------------------------------------------
//TODO: REMOVE THIS FILE AS THIS PACKET ISN'T USED!!!!!!
//-------------------------------------------------------
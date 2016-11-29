#pragma once

#include "packetfactory.h"
#include "entityComponents.h"


namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKWC_PARTY_REPLY, SrvPartyReply)
class SrvPartyReply : public CRosePacket {
	public:
		SrvPartyReply();
		SrvPartyReply(uint8_t reply, uint32_t id);

		virtual ~SrvPartyReply() = default;

		uint8_t reply() const;
		uint32_t id() const;

	protected:
		virtual void pack() override;

	private:
		uint8_t reply_;
		uint32_t id_;
};

}
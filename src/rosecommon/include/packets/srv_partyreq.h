#pragma once

#include "packetfactory.h"
#include "entityComponents.h"


namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKWC_PARTY_REQ, SrvPartyReq)
class SrvPartyReq : public CRosePacket {
	public:
		SrvPartyReq();
		SrvPartyReq(uint8_t request, Entity entity);

		virtual ~SrvPartyReq() = default;

		uint8_t request() const;
		Entity entity() const;

	protected:
		virtual void pack() override;

	private:
		uint8_t request_;
		Entity entity_;
};

}
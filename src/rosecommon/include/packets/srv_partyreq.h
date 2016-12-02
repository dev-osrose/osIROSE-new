#pragma once

#include "packetfactory.h"
#include "entityComponents.h"


namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKWC_PARTY_REQ, SrvPartyReq)
class SrvPartyReq : public CRosePacket {
	public:
        enum Request : uint8_t {
            MAKE = 0,
            JOIN = 1,
            LEFT,
            CHANGE_OWNER,
            BAN = 0x81
        };

		SrvPartyReq();
		SrvPartyReq(Request request, Entity entity);

		virtual ~SrvPartyReq() = default;

		Request request() const;
		Entity entity() const;

	protected:
		virtual void pack() override;

	private:
		Request request_;
		Entity entity_;
};

}

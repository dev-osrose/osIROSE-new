#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKWC_PARTY_REPLY, SrvPartyReply)
class SrvPartyReply : public CRosePacket {
	public:
        enum Reply : uint8_t {
            NOT_FOUND = 0,
            BUSY = 1,
            ACCEPT_MAKE,
            ACCEPT_JOIN,
            REJECT,
            DESTROY,
            FULL,
            INVALID_LEVEL,
            CHANGE_OWNER,
            CHANGE_OWNER_DISCONNECT,
            NO_CHANGE_TARGET,
            KICK = 0x80,
            DISCONNECT,
            REJOIN
        };

		SrvPartyReply();
        SrvPartyReply(Reply reply, uint32_t id);
		SrvPartyReply(Reply reply, Entity entity);

		virtual ~SrvPartyReply() = default;

		Reply reply() const;
        uint32_t id() const;

	protected:
		virtual void pack() override;

	private:
		Reply reply_;
        uint32_t id_;
};

}

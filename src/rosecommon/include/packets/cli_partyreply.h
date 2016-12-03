#pragma once

#include "packetfactory.h"
#include "entityComponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_PARTY_REPLY, CliPartyReply)
class CliPartyReply : public CRosePacket {
	public:
        enum Reply : uint8_t {
            NOT_FOUND = 0,
            BUSY = 1,
            ACCEPT_MAKE,
            ACCEPT_JOIN,
            REJECT,
            DESTROY,
            FULL_MEMBERS,
            INVALID_LEVEL,
            CHANGE_OWNER,
            CHANGE_OWNER_DISCONNECT,
            NO_CHANGE_TARGET,
            KICK = 0x80,
            DISCONNECT,
            REJOIN
        };

		CliPartyReply();
		CliPartyReply(uint8_t buffer[MAX_PACKET_SIZE]);
		CliPartyReply(Reply reply, uint32_t idXorTag);

		virtual ~CliPartyReply() = default;

		Reply reply() const;
		uint32_t idXorTag() const;

	protected:
		virtual void pack() override;

	private:
		Reply reply_;
		uint32_t idXorTag_;
};

}

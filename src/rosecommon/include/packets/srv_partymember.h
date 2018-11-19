#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <dataconsts.h>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKWC_PARTY_MEMBER, SrvPartyMember)
REGISTER_SEND_PACKET(ePacketType::PAKWC_PARTY_MEMBER, SrvPartyMember)
class SrvPartyMember : public CRosePacket {
	public:
		SrvPartyMember();
		SrvPartyMember(CRoseReader reader);
	private:
		SrvPartyMember(uint8_t rules, PartyMember::Party party);
	public:

		virtual ~SrvPartyMember() = default;

		uint8_t rules() const;
		SrvPartyMember& set_rules(uint8_t);
		PartyMember::Party party() const;
		SrvPartyMember& set_party(PartyMember::Party);

		static SrvPartyMember create(uint8_t rules, PartyMember::Party party);
		static SrvPartyMember create(uint8_t *buffer);
		static std::unique_ptr<SrvPartyMember> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		uint8_t rules_;
		PartyMember::Party party_;
};

}
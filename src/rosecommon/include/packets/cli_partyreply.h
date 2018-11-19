#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <dataconsts.h>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_PARTY_REPLY, CliPartyReply)
REGISTER_SEND_PACKET(ePacketType::PAKCS_PARTY_REPLY, CliPartyReply)
class CliPartyReply : public CRosePacket {
	public:
		CliPartyReply();
		CliPartyReply(CRoseReader reader);
	private:
		CliPartyReply(PartyReply::Reply reply, uint32_t idXorTag);
	public:

		virtual ~CliPartyReply() = default;

		PartyReply::Reply reply() const;
		CliPartyReply& set_reply(PartyReply::Reply);
		uint32_t idXorTag() const;
		CliPartyReply& set_idXorTag(uint32_t);

		static CliPartyReply create(PartyReply::Reply reply, uint32_t idXorTag);
		static CliPartyReply create(uint8_t *buffer);
		static std::unique_ptr<CliPartyReply> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		PartyReply::Reply reply_;
		uint32_t idXorTag_;
};

}
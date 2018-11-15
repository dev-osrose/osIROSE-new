#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKWC_PARTY_REPLY, SrvPartyReply)
REGISTER_SEND_PACKET(ePacketType::PAKWC_PARTY_REPLY, SrvPartyReply)
class SrvPartyReply : public CRosePacket {
	public:
		SrvPartyReply();
		SrvPartyReply(CRoseReader reader);
	private:
		SrvPartyReply(PartyReply::Reply reply, uint32_t id);
	public:

		virtual ~SrvPartyReply() = default;

		PartyReply::Reply reply() const;
		SrvPartyReply& set_reply(PartyReply::Reply);
		uint32_t id() const;
		SrvPartyReply& set_id(uint32_t);

		static SrvPartyReply create(PartyReply::Reply reply, uint32_t id);
		static SrvPartyReply create(uint8_t *buffer);

	protected:
		virtual void pack(CRoseWriter&) const override;
		virtual uint16_t get_size() const override;

	private:
		PartyReply::Reply reply_;
		uint32_t id_;
};

}
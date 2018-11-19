#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <dataconsts.h>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_PARTY_REQ, CliPartyReq)
REGISTER_SEND_PACKET(ePacketType::PAKCS_PARTY_REQ, CliPartyReq)
class CliPartyReq : public CRosePacket {
	public:
		CliPartyReq();
		CliPartyReq(CRoseReader reader);
	private:
		CliPartyReq(PartyReq::Request request, uint32_t idXorTag);
	public:

		virtual ~CliPartyReq() = default;

		PartyReq::Request request() const;
		CliPartyReq& set_request(PartyReq::Request);
		uint32_t idXorTag() const;
		CliPartyReq& set_idXorTag(uint32_t);

		static CliPartyReq create(PartyReq::Request request, uint32_t idXorTag);
		static CliPartyReq create(uint8_t *buffer);
		static std::unique_ptr<CliPartyReq> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		PartyReq::Request request_;
		uint32_t idXorTag_;
};

}
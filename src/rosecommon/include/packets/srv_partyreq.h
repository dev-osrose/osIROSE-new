#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <dataconsts.h>
#include <string>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKWC_PARTY_REQ, SrvPartyReq)
REGISTER_SEND_PACKET(ePacketType::PAKWC_PARTY_REQ, SrvPartyReq)
class SrvPartyReq : public CRosePacket {
	public:
		SrvPartyReq();
		SrvPartyReq(CRoseReader reader);
	private:
		SrvPartyReq(PartyReq::Request request, uint32_t tag, const std::string& name);
	public:

		virtual ~SrvPartyReq() = default;

		PartyReq::Request request() const;
		SrvPartyReq& set_request(PartyReq::Request);
		uint32_t tag() const;
		SrvPartyReq& set_tag(uint32_t);
		const std::string& name() const;
		SrvPartyReq& set_name(const std::string&);

		static SrvPartyReq create(PartyReq::Request request, Entity entity);
		static SrvPartyReq create(uint8_t *buffer);
		static std::unique_ptr<SrvPartyReq> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		PartyReq::Request request_;
		uint32_t tag_;
		std::string name_;
};

}
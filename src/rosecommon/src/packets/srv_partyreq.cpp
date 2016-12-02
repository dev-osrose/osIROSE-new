#include "srv_partyreq.h"

namespace RoseCommon {

SrvPartyReq::SrvPartyReq() : CRosePacket(ePacketType::PAKWC_PARTY_REQ) {}

SrvPartyReq::SrvPartyReq(SrvPartyReq::Request request, Entity entity) : CRosePacket(ePacketType::PAKWC_PARTY_REQ), request_(request), entity_(entity) {}

SrvPartyReq::Request SrvPartyReq::request() const {
	return request_;
}

Entity SrvPartyReq::entity() const {
	return entity_;
}


void SrvPartyReq::pack() {
    auto basicInfo = entity_.component<BasicInfo>();

	*this << to_underlying(request_);
	*this << basicInfo->tag_;
	*this << basicInfo->name_;

}

}

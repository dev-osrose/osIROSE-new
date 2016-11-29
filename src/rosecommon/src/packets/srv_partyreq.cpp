#include "srv_partyreq.h"

namespace RoseCommon {

SrvPartyReq::SrvPartyReq() : CRosePacket(ePacketType::PAKWC_PARTY_REQ) {}

SrvPartyReq::SrvPartyReq(uint8_t request, Entity entity) : CRosePacket(ePacketType::PAKWC_PARTY_REQ), request_(request), entity_(entity) {}

uint8_t SrvPartyReq::request() const {
	return request_;
}

Entity SrvPartyReq::entity() const {
	return entity_;
}


void SrvPartyReq::pack() {
    auto basicInfo = entity_.component<BasicInfo>();

	*this << request_;
	*this << basicInfo->id_;
	*this << basicInfo->name_;

}

}

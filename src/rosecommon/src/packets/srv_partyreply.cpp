#include "srv_partyreply.h"

namespace RoseCommon {

SrvPartyReply::SrvPartyReply() : CRosePacket(ePacketType::PAKWC_PARTY_REPLY) {}

SrvPartyReply::SrvPartyReply(SrvPartyReply::Reply reply, Entity entity) : CRosePacket(ePacketType::PAKWC_PARTY_REPLY), reply_(reply), id_(entity.component<BasicInfo>()->tag_) {}

SrvPartyReply::SrvPartyReply(SrvPartyReply::Reply reply, uint32_t id) : CRosePacket(ePacketType::PAKWC_PARTY_REPLY), reply_(reply), id_(id) {}

SrvPartyReply::Reply SrvPartyReply::reply() const {
	return reply_;
}

uint32_t SrvPartyReply::id() const {
	return id_;
}


void SrvPartyReply::pack() {
	*this << to_underlying(reply_);
	*this << id_;
}

}

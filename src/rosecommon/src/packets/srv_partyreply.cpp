#include "srv_partyreply.h"

namespace RoseCommon {

SrvPartyReply::SrvPartyReply() : CRosePacket(ePacketType::PAKWC_PARTY_REPLY) {}

SrvPartyReply::SrvPartyReply(uint8_t reply, uint32_t id) : CRosePacket(ePacketType::PAKWC_PARTY_REPLY), reply_(reply), id_(id) {}

uint8_t SrvPartyReply::reply() const {
	return reply_;
}

uint32_t SrvPartyReply::id() const {
	return id_;
}


void SrvPartyReply::pack() {
	*this << reply_;
	*this << id_;
}

}
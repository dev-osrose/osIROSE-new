#include "srv_joinserverreply.h"

namespace RoseCommon {

SrvJoinServerReply::SrvJoinServerReply() : CRosePacket(ePacketType::PAKSC_JOIN_SERVER_REPLY) {}

SrvJoinServerReply::SrvJoinServerReply(SrvJoinServerReply::eResult result, uint32_t id, uint32_t payFlag) : CRosePacket(ePacketType::PAKSC_JOIN_SERVER_REPLY), result_(result), id_(id), payFlag_(payFlag) {}

SrvJoinServerReply::eResult SrvJoinServerReply::result() const {
	return result_;
}

uint32_t SrvJoinServerReply::id() const {
	return id_;
}

uint32_t SrvJoinServerReply::payFlag() const {
	return payFlag_;
}


void SrvJoinServerReply::pack() {
    *this << to_underlying(result_);
	*this << id_;
	*this << payFlag_;
}

}

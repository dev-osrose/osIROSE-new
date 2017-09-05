#include "srv_joinserverreply.h"
#include "throwassert.h"

namespace RoseCommon {

SrvJoinServerReply::SrvJoinServerReply() : CRosePacket(ePacketType::PAKSC_JOIN_SERVER_REPLY) {}

  SrvJoinServerReply::SrvJoinServerReply(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    throw_assert(type() == ePacketType::PAKSC_JOIN_SERVER_REPLY, "Not the right packet: " << to_underlying(type()));
    *this >> (uint8_t&)result_;
    *this >> id_;
    *this >> payFlag_;
  }

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

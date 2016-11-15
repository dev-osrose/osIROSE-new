#include "srv_acceptreply.h"

namespace RoseCommon {

SrvAcceptReply::SrvAcceptReply() : CRosePacket(ePacketType::PAKSS_ACCEPT_REPLY) {}

SrvAcceptReply::SrvAcceptReply(uint32_t randValue, uint8_t result) : CRosePacket(ePacketType::PAKSS_ACCEPT_REPLY), result_(result), randValue_(randValue) {}

uint8_t SrvAcceptReply::result() const {
	return result_;
}

uint32_t SrvAcceptReply::randValue() const {
	return randValue_;
}


void SrvAcceptReply::pack() {
	*this << result_;
	*this << randValue_;
}

}

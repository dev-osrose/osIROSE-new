#include "srv_acceptreply.h"

namespace RoseCommon {

SrvAcceptReply::SrvAcceptReply() : CRosePacket(ePacketType::PAKSS_ACCEPT_REPLY) {
}

SrvAcceptReply::SrvAcceptReply(uint8_t result, uint32_t randValue) : CRosePacket(ePacketType::PAKSS_ACCEPT_REPLY), result_(result), randValue_(randValue) {
}

uint8_t &SrvAcceptReply::result() {
	return result_;
}

uint32_t &SrvAcceptReply::randValue() {
	return randValue_;
}

void SrvAcceptReply::pack() {
	*this << result_;
	*this << randValue_;
}

}
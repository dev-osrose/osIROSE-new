#include "srv_createcharreply.h"

namespace RoseCommon {

SrvCreateCharReply::SrvCreateCharReply() : CRosePacket(ePacketType::PAKCC_CREATE_CHAR_REPLY) {
}

SrvCreateCharReply::SrvCreateCharReply(eResult result, uint8_t platinium) : CRosePacket(ePacketType::PAKCC_CREATE_CHAR_REPLY), result_(result), platinium_(platinium) {
}

SrvCreateCharReply::eResult &SrvCreateCharReply::result() {
	return result_;
}

uint8_t &SrvCreateCharReply::platinium() {
	return platinium_;
}

void SrvCreateCharReply::pack() {
	*this << result_;
	*this << platinium_;
}

}

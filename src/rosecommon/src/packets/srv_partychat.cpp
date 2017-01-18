#include "srv_partychat.h"

namespace RoseCommon {

SrvPartyChat::SrvPartyChat() : CRosePacket(ePacketType::PAKWC_PARTY_CHAT) {}

SrvPartyChat::SrvPartyChat(uint16_t charId, const std::string &message) : CRosePacket(ePacketType::PAKWC_PARTY_CHAT), charId_(charId), message_(message) {}

uint16_t SrvPartyChat::charId() const {
	return charId_;
}

std::string &SrvPartyChat::message() {
	return message_;
}

const std::string &SrvPartyChat::message() const {
	return message_;
}


void SrvPartyChat::pack() {
	*this << charId_;
	*this << message_;
}

}
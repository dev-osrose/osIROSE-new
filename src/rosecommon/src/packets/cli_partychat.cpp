#include "cli_partychat.h"

namespace RoseCommon {

CliPartyChat::CliPartyChat() : CRosePacket(ePacketType::PAKCS_PARTY_CHAT) {}

CliPartyChat::CliPartyChat(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	if (type() != ePacketType::PAKCS_PARTY_CHAT)
		throw std::runtime_error("Not the right packet!");
	*this >> message_;
}

CliPartyChat::CliPartyChat(const std::string &message) : CRosePacket(ePacketType::PAKCS_PARTY_CHAT), message_(message) {}

std::string &CliPartyChat::message() {
	return message_;
}

const std::string &CliPartyChat::message() const {
	return message_;
}


void CliPartyChat::pack() {
	*this << message_;
}

}
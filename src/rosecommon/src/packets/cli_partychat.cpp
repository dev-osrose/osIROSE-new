#include "cli_partychat.h"
#include "throwassert.h"

namespace RoseCommon {

const RecvPacketFactory::Initializer<uint8_t*> CliPartyChat::init = RecvPacketFactory::Initializer<uint8_t*>(ePacketType::PAKCS_PARTY_CHAT, &createPacket<CliPartyChat>);

CliPartyChat::CliPartyChat() : CRosePacket(ePacketType::PAKCS_PARTY_CHAT) {}

CliPartyChat::CliPartyChat(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	throw_assert(type() == ePacketType::PAKCS_PARTY_CHAT, "Not the right packet: " << to_underlying(type()));
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

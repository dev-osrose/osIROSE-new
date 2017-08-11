#include "cli_normalchat.h"
#include "throwassert.h"

namespace RoseCommon {

const RecvPacketFactory::Initializer<uint8_t[MAX_PACKET_SIZE]> CliNormalChat::init = RecvPacketFactory::Initializer<uint8_t[MAX_PACKET_SIZE]>(ePacketType::PAKCS_NORMAL_CHAT, &createPacket<CliNormalChat>);

CliNormalChat::CliNormalChat() : CRosePacket(ePacketType::PAKCS_NORMAL_CHAT) {}

CliNormalChat::CliNormalChat(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	throw_assert(type() == ePacketType::PAKCS_NORMAL_CHAT, "Not the right packet: " << to_underlying(type()));
	*this >> message_;
}

CliNormalChat::CliNormalChat(const std::string &message) : CRosePacket(ePacketType::PAKCS_NORMAL_CHAT), message_(message) {}

std::string &CliNormalChat::message() {
	return message_;
}

const std::string &CliNormalChat::message() const {
	return message_;
}


void CliNormalChat::pack() {
	*this << message_;
}

}
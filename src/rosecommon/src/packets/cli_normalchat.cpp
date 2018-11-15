#include "cli_normalchat.h"
#include "throwassert.h"

namespace RoseCommon {

CliNormalChat::CliNormalChat() : CRosePacket(ePacketType::PAKCS_NORMAL_CHAT) {}

CliNormalChat::CliNormalChat(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKCS_NORMAL_CHAT, "Not the right packet: " << to_underlying(get_type()));
	reader.get_string(message_);
}

CliNormalChat::CliNormalChat(const std::string& message) : CRosePacket(ePacketType::PAKCS_NORMAL_CHAT), message_(message) {
}

const std::string& CliNormalChat::message() const { return message_; }

CliNormalChat& CliNormalChat::set_message(const std::string& data) { message_ = data; return *this; }


void CliNormalChat::pack(CRoseWriter& writer) const {
	writer.set_string(message_);
}

uint16_t CliNormalChat::get_size() const {
	uint16_t size = 0;
	size += sizeof(char) * (message_.size() + 1);
	return size;
}


CliNormalChat CliNormalChat::create(const std::string& message) {


	return CliNormalChat(message);
}

CliNormalChat CliNormalChat::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return CliNormalChat(reader);
}

}
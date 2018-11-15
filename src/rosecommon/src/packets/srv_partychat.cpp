#include "srv_partychat.h"
#include "throwassert.h"

namespace RoseCommon {

SrvPartyChat::SrvPartyChat() : CRosePacket(ePacketType::PAKWC_PARTY_CHAT) {}

SrvPartyChat::SrvPartyChat(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKWC_PARTY_CHAT, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint16_t(charId_);
	reader.get_string(message_);
}

SrvPartyChat::SrvPartyChat(uint16_t charId, const std::string& message) : CRosePacket(ePacketType::PAKWC_PARTY_CHAT), charId_(charId), message_(message) {
}

uint16_t SrvPartyChat::charId() const { return charId_; }

SrvPartyChat& SrvPartyChat::set_charId(uint16_t data) { charId_ = data; return *this; }

const std::string& SrvPartyChat::message() const { return message_; }

SrvPartyChat& SrvPartyChat::set_message(const std::string& data) { message_ = data; return *this; }


void SrvPartyChat::pack(CRoseWriter& writer) const {
	writer.set_uint16_t(charId_);
	writer.set_string(message_);
}

uint16_t SrvPartyChat::get_size() const {
	uint16_t size = 0;
	size += sizeof(charId_);
	size += sizeof(char) * (message_.size() + 1);
	return size;
}


SrvPartyChat SrvPartyChat::create(uint16_t charId, const std::string& message) {


	return SrvPartyChat(charId, message);
}

SrvPartyChat SrvPartyChat::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvPartyChat(reader);
}

}
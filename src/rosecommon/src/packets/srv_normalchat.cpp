#include "srv_normalchat.h"
#include "throwassert.h"

namespace RoseCommon {

SrvNormalChat::SrvNormalChat() : CRosePacket(ePacketType::PAKWC_NORMAL_CHAT) {}

SrvNormalChat::SrvNormalChat(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKWC_NORMAL_CHAT, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint16_t(charId_);
	reader.get_string(message_);
}

SrvNormalChat::SrvNormalChat(uint16_t charId, const std::string& message) : CRosePacket(ePacketType::PAKWC_NORMAL_CHAT), charId_(charId), message_(message) {
}

uint16_t SrvNormalChat::charId() const { return charId_; }

SrvNormalChat& SrvNormalChat::set_charId(uint16_t data) { charId_ = data; return *this; }

const std::string& SrvNormalChat::message() const { return message_; }

SrvNormalChat& SrvNormalChat::set_message(const std::string& data) { message_ = data; return *this; }


void SrvNormalChat::pack(CRoseWriter& writer) const {
	writer.set_uint16_t(charId_);
	writer.set_string(message_);
}

uint16_t SrvNormalChat::get_size() const {
	uint16_t size = 0;
	size += sizeof(charId_);
	size += sizeof(char) * (message_.size() + 1);
	return size;
}


SrvNormalChat SrvNormalChat::create(uint16_t charId, const std::string& message) {


	return SrvNormalChat(charId, message);
}

SrvNormalChat SrvNormalChat::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvNormalChat(reader);
}

}
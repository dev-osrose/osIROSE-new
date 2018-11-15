#include "srv_whisperchat.h"
#include "throwassert.h"

namespace RoseCommon {

SrvWhisperChat::SrvWhisperChat() : CRosePacket(ePacketType::PAKWC_WHISPER_CHAT) {}

SrvWhisperChat::SrvWhisperChat(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKWC_WHISPER_CHAT, "Not the right packet: " << to_underlying(get_type()));
	reader.get_string(senderId_);
	reader.get_string(message_);
}

SrvWhisperChat::SrvWhisperChat(const std::string& senderId, const std::string& message) : CRosePacket(ePacketType::PAKWC_WHISPER_CHAT), senderId_(senderId), message_(message) {
}

const std::string& SrvWhisperChat::senderId() const { return senderId_; }

SrvWhisperChat& SrvWhisperChat::set_senderId(const std::string& data) { senderId_ = data; return *this; }

const std::string& SrvWhisperChat::message() const { return message_; }

SrvWhisperChat& SrvWhisperChat::set_message(const std::string& data) { message_ = data; return *this; }


void SrvWhisperChat::pack(CRoseWriter& writer) const {
	writer.set_string(senderId_);
	writer.set_string(message_);
}

uint16_t SrvWhisperChat::get_size() const {
	uint16_t size = 0;
	size += sizeof(char) * (senderId_.size() + 1);
	size += sizeof(char) * (message_.size() + 1);
	return size;
}


SrvWhisperChat SrvWhisperChat::create(const std::string& senderId, const std::string& message) {


	return SrvWhisperChat(senderId, message);
}

SrvWhisperChat SrvWhisperChat::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvWhisperChat(reader);
}

}
#include "cli_whisperchat.h"
#include "throwassert.h"

namespace RoseCommon {

CliWhisperChat::CliWhisperChat() : CRosePacket(ePacketType::PAKCS_WHISPER_CHAT) {}

CliWhisperChat::CliWhisperChat(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKCS_WHISPER_CHAT, "Not the right packet: " << to_underlying(get_type()));
	reader.get_string(targetId_);
	reader.get_string(message_);
}

CliWhisperChat::CliWhisperChat(const std::string& targetId, const std::string& message) : CRosePacket(ePacketType::PAKCS_WHISPER_CHAT), targetId_(targetId), message_(message) {
}

const std::string& CliWhisperChat::targetId() const { return targetId_; }

CliWhisperChat& CliWhisperChat::set_targetId(const std::string& data) { targetId_ = data; return *this; }

const std::string& CliWhisperChat::message() const { return message_; }

CliWhisperChat& CliWhisperChat::set_message(const std::string& data) { message_ = data; return *this; }


void CliWhisperChat::pack(CRoseWriter& writer) const {
	writer.set_string(targetId_);
	writer.set_string(message_);
}

uint16_t CliWhisperChat::get_size() const {
	uint16_t size = 0;
	size += sizeof(char) * (targetId_.size() + 1);
	size += sizeof(char) * (message_.size() + 1);
	return size;
}


CliWhisperChat CliWhisperChat::create(const std::string& targetId, const std::string& message) {


	return CliWhisperChat(targetId, message);
}

CliWhisperChat CliWhisperChat::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return CliWhisperChat(reader);
}

}
#include "cli_whisperchat.h"

namespace RoseCommon {

CliWhisperChat::CliWhisperChat() : CRosePacket(ePacketType::PAKCS_WHISPER_CHAT) {}

CliWhisperChat::CliWhisperChat(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	if (type() != ePacketType::PAKCS_WHISPER_CHAT)
		throw std::runtime_error("Not the right packet!");
	*this >> targetId_;
	*this >> message_;
}

CliWhisperChat::CliWhisperChat(const std::string &targetId, const std::string &message) : CRosePacket(ePacketType::PAKCS_WHISPER_CHAT), targetId_(targetId), message_(message) {}

std::string &CliWhisperChat::targetId() {
	return targetId_;
}

const std::string &CliWhisperChat::targetId() const {
	return targetId_;
}

std::string &CliWhisperChat::message() {
	return message_;
}

const std::string &CliWhisperChat::message() const {
	return message_;
}


void CliWhisperChat::pack() {
	*this << targetId_;
	*this << message_;
}

}
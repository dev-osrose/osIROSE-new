#include "srv_whisperchat.h"

namespace RoseCommon {

SrvWhisperChat::SrvWhisperChat() : CRosePacket(ePacketType::PAKWC_WHISPER_CHAT) {}

SrvWhisperChat::SrvWhisperChat(const std::string &senderId, const std::string &message) : CRosePacket(ePacketType::PAKWC_WHISPER_CHAT), senderId_(senderId), message_(message) {}

std::string &SrvWhisperChat::senderId() {
	return senderId_;
}

const std::string &SrvWhisperChat::senderId() const {
	return senderId_;
}

std::string &SrvWhisperChat::message() {
	return message_;
}

const std::string &SrvWhisperChat::message() const {
	return message_;
}


void SrvWhisperChat::pack() {
	*this << senderId_;
	*this << message_;
}

}
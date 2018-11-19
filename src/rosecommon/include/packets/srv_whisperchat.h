#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <string>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKWC_WHISPER_CHAT, SrvWhisperChat)
REGISTER_SEND_PACKET(ePacketType::PAKWC_WHISPER_CHAT, SrvWhisperChat)
class SrvWhisperChat : public CRosePacket {
	public:
		SrvWhisperChat();
		SrvWhisperChat(CRoseReader reader);
	private:
		SrvWhisperChat(const std::string& senderId, const std::string& message);
	public:

		virtual ~SrvWhisperChat() = default;

		const std::string& senderId() const;
		SrvWhisperChat& set_senderId(const std::string&);
		const std::string& message() const;
		SrvWhisperChat& set_message(const std::string&);

		static SrvWhisperChat create(const std::string& senderId, const std::string& message);
		static SrvWhisperChat create(uint8_t *buffer);
		static std::unique_ptr<SrvWhisperChat> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		std::string senderId_;
		std::string message_;
};

}
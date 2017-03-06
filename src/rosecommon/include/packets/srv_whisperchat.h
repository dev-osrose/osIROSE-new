#pragma once

#include "packetfactory.h"
#include "entityComponents.h"
#include <string>

namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKWC_WHISPER_CHAT, SrvWhisperChat)
class SrvWhisperChat : public CRosePacket {
	public:
		SrvWhisperChat();
		SrvWhisperChat(const std::string &senderId, const std::string &message);

		virtual ~SrvWhisperChat() = default;

		std::string &senderId();
		const std::string &senderId() const;
		std::string &message();
		const std::string &message() const;

	protected:
		virtual void pack() override;

	private:
		std::string senderId_;
		std::string message_;
};

}
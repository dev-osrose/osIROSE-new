#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <string>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_WHISPER_CHAT, CliWhisperChat)
class CliWhisperChat : public CRosePacket {
	public:
		CliWhisperChat();
		CliWhisperChat(uint8_t buffer[MAX_PACKET_SIZE]);
		CliWhisperChat(const std::string &targetId, const std::string &message);

		virtual ~CliWhisperChat() = default;

		std::string &targetId();
		const std::string &targetId() const;
		std::string &message();
		const std::string &message() const;

	protected:
		virtual void pack() override;

	private:
		std::string targetId_;
		std::string message_;
};

}
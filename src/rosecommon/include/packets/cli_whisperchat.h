#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <string>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_WHISPER_CHAT, CliWhisperChat)
REGISTER_SEND_PACKET(ePacketType::PAKCS_WHISPER_CHAT, CliWhisperChat)
class CliWhisperChat : public CRosePacket {
	public:
		CliWhisperChat();
		CliWhisperChat(CRoseReader reader);
	private:
		CliWhisperChat(const std::string& targetId, const std::string& message);
	public:

		virtual ~CliWhisperChat() = default;

		const std::string& targetId() const;
		CliWhisperChat& set_targetId(const std::string&);
		const std::string& message() const;
		CliWhisperChat& set_message(const std::string&);

		static CliWhisperChat create(const std::string& targetId, const std::string& message);
		static CliWhisperChat create(uint8_t *buffer);

	protected:
		virtual void pack(CRoseWriter&) const override;
		virtual uint16_t get_size() const override;

	private:
		std::string targetId_;
		std::string message_;
};

}
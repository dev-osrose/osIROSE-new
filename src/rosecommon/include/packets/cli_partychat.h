#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <string>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_PARTY_CHAT, CliPartyChat)
REGISTER_SEND_PACKET(ePacketType::PAKCS_PARTY_CHAT, CliPartyChat)
class CliPartyChat : public CRosePacket {
	public:
		CliPartyChat();
		CliPartyChat(CRoseReader reader);
	private:
		CliPartyChat(const std::string& message);
	public:

		virtual ~CliPartyChat() = default;

		const std::string& message() const;
		CliPartyChat& set_message(const std::string&);

		static CliPartyChat create(const std::string& message);
		static CliPartyChat create(uint8_t *buffer);

	protected:
		virtual void pack(CRoseWriter&) const override;
		virtual uint16_t get_size() const override;

	private:
		std::string message_;
};

}
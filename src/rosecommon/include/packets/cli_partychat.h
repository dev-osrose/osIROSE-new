#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <string>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_PARTY_CHAT, CliPartyChat)
class CliPartyChat : public CRosePacket {
	public:
		CliPartyChat();
		CliPartyChat(uint8_t buffer[MAX_PACKET_SIZE]);
		CliPartyChat(const std::string &message);

		virtual ~CliPartyChat() = default;

		std::string &message();
		const std::string &message() const;

	protected:
		virtual void pack() override;

	private:
		std::string message_;
};

}
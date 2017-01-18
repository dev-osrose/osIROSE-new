#pragma once

#include "packetfactory.h"
#include "entityComponents.h"
#include <string>

namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKWC_PARTY_CHAT, SrvPartyChat)
class SrvPartyChat : public CRosePacket {
	public:
		SrvPartyChat();
		SrvPartyChat(uint16_t charId, const std::string &message);

		virtual ~SrvPartyChat() = default;

		uint16_t charId() const;
		std::string &message();
		const std::string &message() const;

	protected:
		virtual void pack() override;

	private:
		uint16_t charId_;
		std::string message_;
};

}
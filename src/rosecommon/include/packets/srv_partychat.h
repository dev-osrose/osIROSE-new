#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <string>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKWC_PARTY_CHAT, SrvPartyChat)
REGISTER_SEND_PACKET(ePacketType::PAKWC_PARTY_CHAT, SrvPartyChat)
class SrvPartyChat : public CRosePacket {
	public:
		SrvPartyChat();
		SrvPartyChat(CRoseReader reader);
	private:
		SrvPartyChat(uint16_t charId, const std::string& message);
	public:

		virtual ~SrvPartyChat() = default;

		uint16_t charId() const;
		SrvPartyChat& set_charId(uint16_t);
		const std::string& message() const;
		SrvPartyChat& set_message(const std::string&);

		static SrvPartyChat create(uint16_t charId, const std::string& message);
		static SrvPartyChat create(uint8_t *buffer);

	protected:
		virtual void pack(CRoseWriter&) const override;
		virtual uint16_t get_size() const override;

	private:
		uint16_t charId_;
		std::string message_;
};

}
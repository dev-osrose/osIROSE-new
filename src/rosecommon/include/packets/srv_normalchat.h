#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <string>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKWC_NORMAL_CHAT, SrvNormalChat)
REGISTER_SEND_PACKET(ePacketType::PAKWC_NORMAL_CHAT, SrvNormalChat)
class SrvNormalChat : public CRosePacket {
	public:
		SrvNormalChat();
		SrvNormalChat(CRoseReader reader);
	private:
		SrvNormalChat(uint16_t charId, const std::string& message);
	public:

		virtual ~SrvNormalChat() = default;

		uint16_t charId() const;
		SrvNormalChat& set_charId(uint16_t);
		const std::string& message() const;
		SrvNormalChat& set_message(const std::string&);

		static SrvNormalChat create(uint16_t charId, const std::string& message);
		static SrvNormalChat create(uint8_t *buffer);
		static std::unique_ptr<SrvNormalChat> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		uint16_t charId_;
		std::string message_;
};

}
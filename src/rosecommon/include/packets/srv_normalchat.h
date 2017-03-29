#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKWC_NORMAL_CHAT, SrvNormalChat)
class SrvNormalChat : public CRosePacket {
	public:
		SrvNormalChat();
		SrvNormalChat(uint16_t charId, const std::string &message);

		virtual ~SrvNormalChat() = default;

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
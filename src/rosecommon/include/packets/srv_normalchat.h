#pragma once

#include "packetfactory.h"
#include "entityComponents.h"
#include <string>

namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKWC_NORMAL_CHAT, SrvNormalChat)
class SrvNormalChat : public CRosePacket {
	public:
		SrvNormalChat();

		SrvNormalChat(uint16_t charId, std::string message);

		virtual ~SrvNormalChat() = default;

		uint16_t &charId();
		std::string &message();

	protected:
		virtual void pack() override;

	private:
		uint16_t charId_;
		std::string message_;
};

}
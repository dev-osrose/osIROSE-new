#pragma once

#include "packetfactory.h"
#include "entityComponents.h"
#include <string>

namespace RoseCommon {

class CliNormalChat : public CRosePacket, public RegisterRecvPacket<ePacketType::PAKCS_NORMAL_CHAT, CliNormalChat> {
	public:
		CliNormalChat(uint8_t buffer[MAX_PACKET_SIZE]);

		virtual ~CliNormalChat() = default;

		std::string &message();

	private:
		std::string message_;
};

}

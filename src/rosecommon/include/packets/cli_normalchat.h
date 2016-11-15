#pragma once

#include "packetfactory.h"
#include "entityComponents.h"
#include <string>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_NORMAL_CHAT, CliNormalChat)
class CliNormalChat : public CRosePacket {
	public:
		CliNormalChat(uint8_t buffer[MAX_PACKET_SIZE]);

		virtual ~CliNormalChat() = default;

		std::string &message();
        const std::string &message() const;

	private:
		std::string message_;
};

}

#pragma once

#include "packetfactory.h"
#include "entityComponents.h"
#include <string>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_JOIN_SERVER_REQ, CliJoinServerReq)
class CliJoinServerReq : public CRosePacket {
	public:
		CliJoinServerReq(uint8_t buffer[MAX_PACKET_SIZE]);
        CliJoinServerReq(uint32_t sessionId, const std::string& password);

        virtual void pack() override;

		virtual ~CliJoinServerReq() = default;

		uint32_t &sessionId();
		std::string &password();

	private:
		uint32_t sessionId_;
		std::string password_;
};

}

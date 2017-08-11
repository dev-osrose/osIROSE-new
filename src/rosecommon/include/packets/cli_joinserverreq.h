#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <string>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_JOIN_SERVER_REQ, CliJoinServerReq)
class CliJoinServerReq : public CRosePacket {
	private:
		static const RecvPacketFactory::Initializer<uint8_t[MAX_PACKET_SIZE]> init;
	public:
		CliJoinServerReq();
		CliJoinServerReq(uint8_t buffer[MAX_PACKET_SIZE]);
		CliJoinServerReq(uint32_t sessionId, const std::string &password);

		virtual ~CliJoinServerReq() = default;

		uint32_t sessionId() const;
		std::string &password();
		const std::string &password() const;

	protected:
		virtual void pack() override;

	private:
		uint32_t sessionId_;
		std::string password_;
};

}
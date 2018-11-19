#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <string>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_JOIN_SERVER_REQ, CliJoinServerReq)
REGISTER_SEND_PACKET(ePacketType::PAKCS_JOIN_SERVER_REQ, CliJoinServerReq)
class CliJoinServerReq : public CRosePacket {
	public:
		CliJoinServerReq();
		CliJoinServerReq(CRoseReader reader);
	private:
		CliJoinServerReq(uint32_t sessionId, const std::string& password);
	public:

		virtual ~CliJoinServerReq() = default;

		uint32_t sessionId() const;
		CliJoinServerReq& set_sessionId(uint32_t);
		const std::string& password() const;
		CliJoinServerReq& set_password(const std::string&);

		static CliJoinServerReq create(uint32_t sessionId, const std::string& password);
		static CliJoinServerReq create(uint8_t *buffer);
		static std::unique_ptr<CliJoinServerReq> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		uint32_t sessionId_;
		std::string password_;
};

}
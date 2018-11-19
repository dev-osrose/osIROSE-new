#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <dataconsts.h>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_REVIVE_REQ, CliReviveReq)
REGISTER_SEND_PACKET(ePacketType::PAKCS_REVIVE_REQ, CliReviveReq)
class CliReviveReq : public CRosePacket {
	public:
		CliReviveReq();
		CliReviveReq(CRoseReader reader);
	private:
		CliReviveReq(ReviveReq::ReviveReq reviveType);
	public:

		virtual ~CliReviveReq() = default;

		ReviveReq::ReviveReq reviveType() const;
		CliReviveReq& set_reviveType(ReviveReq::ReviveReq);

		static CliReviveReq create(ReviveReq::ReviveReq reviveType);
		static CliReviveReq create(uint8_t *buffer);
		static std::unique_ptr<CliReviveReq> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		ReviveReq::ReviveReq reviveType_;
};

}
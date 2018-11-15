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

	protected:
		virtual void pack(CRoseWriter&) const override;
		virtual uint16_t get_size() const override;

	private:
		ReviveReq::ReviveReq reviveType_;
};

}
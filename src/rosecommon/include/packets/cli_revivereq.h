#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <dataconsts.h>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_REVIVE_REQ, CliReviveReq)
class CliReviveReq : public CRosePacket {
	private:
		static const RecvPacketFactory::Initializer<uint8_t[MAX_PACKET_SIZE]> init;
	public:
		CliReviveReq();
		CliReviveReq(uint8_t buffer[MAX_PACKET_SIZE]);
		CliReviveReq(ReviveReq::ReviveReq reviveType);

		virtual ~CliReviveReq() = default;

		ReviveReq::ReviveReq reviveType() const;

	protected:
		virtual void pack() override;

	private:
		ReviveReq::ReviveReq reviveType_;
};

}
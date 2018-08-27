#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <string>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_HP_REQ, CliHpReq)
class CliHpReq : public CRosePacket {
	private:
		static const RecvPacketFactory::Initializer<uint8_t*> init;
	public:
		CliHpReq();
		CliHpReq(uint8_t buffer[MAX_PACKET_SIZE]);
		CliHpReq(uint16_t targetId);

		virtual ~CliHpReq() = default;

		uint16_t targetId() const;

	protected:
		virtual void pack() override;

	private:
		uint16_t targetId_;
};

}

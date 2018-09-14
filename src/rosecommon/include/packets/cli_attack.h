#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <string>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_ATTACK, CliAttack)
class CliAttack : public CRosePacket {
	private:
		static const RecvPacketFactory::Initializer<uint8_t*> init;
	public:
		CliAttack();
		CliAttack(uint8_t buffer[MAX_PACKET_SIZE]);
		CliAttack(uint16_t targetId);

		virtual ~CliAttack() = default;

		uint16_t targetId() const;
		void targetId(uint16_t id);

	protected:
		virtual void pack() override;

	private:
		uint16_t targetId_;
};

}

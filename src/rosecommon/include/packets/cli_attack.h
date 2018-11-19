#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_ATTACK, CliAttack)
REGISTER_SEND_PACKET(ePacketType::PAKCS_ATTACK, CliAttack)
class CliAttack : public CRosePacket {
	public:
		CliAttack();
		CliAttack(CRoseReader reader);
	private:
		CliAttack(uint16_t targetId);
	public:

		virtual ~CliAttack() = default;

		uint16_t targetId() const;
		CliAttack& set_targetId(uint16_t);

		static CliAttack create(uint16_t targetId);
		static CliAttack create(uint8_t *buffer);
		static std::unique_ptr<CliAttack> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		uint16_t targetId_;
};

}
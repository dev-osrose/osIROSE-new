#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKWC_DAMAGE, SrvDamage)
REGISTER_SEND_PACKET(ePacketType::PAKWC_DAMAGE, SrvDamage)
class SrvDamage : public CRosePacket {
	public:
		SrvDamage();
		SrvDamage(CRoseReader reader);
	private:
		SrvDamage(uint16_t idA, uint16_t idD, uint32_t damage, uint32_t action);
	public:

		virtual ~SrvDamage() = default;

		uint16_t idA() const;
		SrvDamage& set_idA(uint16_t);
		uint16_t idD() const;
		SrvDamage& set_idD(uint16_t);
		uint32_t damage() const;
		SrvDamage& set_damage(uint32_t);
		uint32_t action() const;
		SrvDamage& set_action(uint32_t);

		static SrvDamage create(Entity entityA, Entity entityD, uint32_t damage, uint32_t action);
		static SrvDamage create(uint8_t *buffer);

	protected:
		virtual void pack(CRoseWriter&) const override;
		virtual uint16_t get_size() const override;

	private:
		uint16_t idA_;
		uint16_t idD_;
		uint32_t damage_;
		uint32_t action_;
};

}
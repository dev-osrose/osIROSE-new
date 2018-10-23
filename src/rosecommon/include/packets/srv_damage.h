#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <string>

namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKWC_DAMAGE, SrvDamage)
class SrvDamage : public CRosePacket {
	public:
		SrvDamage();
		SrvDamage(Entity attacker, Entity defender, uint32_t damage, uint32_t action);

		virtual ~SrvDamage() = default;

		Entity attacker() const;
		Entity defender() const;
		uint32_t damage() const;
		uint32_t action() const;

	protected:
		virtual void pack() override;

	private:
		Entity attacker_;
		Entity defender_;
		uint32_t damage_;
		uint32_t action_;
		//TODO: Add Item info here
};

}
#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <string>

namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKWC_ATTACK, SrvAttack)
class SrvAttack : public CRosePacket {
	public:
		SrvAttack();
		SrvAttack(Entity attacker, Entity defender);

		virtual ~SrvAttack() = default;

		Entity attacker() const;
		Entity defender() const;

	protected:
		virtual void pack() override;

	private:
		Entity attacker_;
		Entity defender_;
};

}
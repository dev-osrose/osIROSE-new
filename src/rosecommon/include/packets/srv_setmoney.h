#pragma once

#include "packetfactory.h"
#include "entityComponents.h"


namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKWC_SET_MONEY, SrvSetMoney)
class SrvSetMoney : public CRosePacket {
	public:
		SrvSetMoney();
		SrvSetMoney(Entity entity);

		virtual ~SrvSetMoney() = default;

		Entity entity() const;

	protected:
		virtual void pack() override;

	private:
		Entity entity_;
};

}
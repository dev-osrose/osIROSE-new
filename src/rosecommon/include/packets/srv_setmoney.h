#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKWC_SET_MONEY, SrvSetMoney)
class SrvSetMoney : public CRosePacket {
	public:
		SrvSetMoney();
		SrvSetMoney(uint8_t buffer[MAX_PACKET_SIZE]);
		SrvSetMoney(Entity entity);

		virtual ~SrvSetMoney() = default;

		Entity entity() const;

	protected:
		virtual void pack() override;

	private:
		Entity entity_;
};

}
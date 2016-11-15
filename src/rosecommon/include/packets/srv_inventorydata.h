#pragma once

#include "packetfactory.h"
#include "entityComponents.h"


namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKWC_INVENTORY_DATA, SrvInventoryData)
class SrvInventoryData : public CRosePacket {
	public:
		SrvInventoryData();
		SrvInventoryData(Entity entity);

		virtual ~SrvInventoryData() = default;

		Entity entity() const;

	protected:
		virtual void pack() override;

	private:
		Entity entity_;
};

}
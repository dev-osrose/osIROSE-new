#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKWC_REMOVE_OBJECT, SrvRemoveObject)
class SrvRemoveObject : public CRosePacket {
	public:
		SrvRemoveObject();
		SrvRemoveObject(Entity entity);

		virtual ~SrvRemoveObject() = default;

		Entity entity() const;

	protected:
		virtual void pack() override;

	private:
		Entity entity_;
};

}
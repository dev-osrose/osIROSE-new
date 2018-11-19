#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKWC_REMOVE_OBJECT, SrvRemoveObject)
REGISTER_SEND_PACKET(ePacketType::PAKWC_REMOVE_OBJECT, SrvRemoveObject)
class SrvRemoveObject : public CRosePacket {
	public:
		SrvRemoveObject();
		SrvRemoveObject(CRoseReader reader);
	private:
		SrvRemoveObject(uint16_t id);
	public:

		virtual ~SrvRemoveObject() = default;

		uint16_t id() const;
		SrvRemoveObject& set_id(uint16_t);

		static SrvRemoveObject create(Entity entity);
		static SrvRemoveObject create(uint8_t *buffer);
		static std::unique_ptr<SrvRemoveObject> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		uint16_t id_;
};

}
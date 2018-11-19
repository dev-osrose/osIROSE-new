#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKWC_DROP_ITEM, SrvDropItem)
REGISTER_SEND_PACKET(ePacketType::PAKWC_DROP_ITEM, SrvDropItem)
class SrvDropItem : public CRosePacket {
	public:
		SrvDropItem();
		SrvDropItem(CRoseReader reader);
	private:
		SrvDropItem(float x, float y, uint16_t header, uint32_t data, uint16_t id, uint16_t ownerId);
	public:

		virtual ~SrvDropItem() = default;

		float x() const;
		SrvDropItem& set_x(float);
		float y() const;
		SrvDropItem& set_y(float);
		uint16_t header() const;
		SrvDropItem& set_header(uint16_t);
		uint32_t data() const;
		SrvDropItem& set_data(uint32_t);
		uint16_t id() const;
		SrvDropItem& set_id(uint16_t);
		uint16_t ownerId() const;
		SrvDropItem& set_ownerId(uint16_t);
		uint16_t timeToLive() const;
		SrvDropItem& set_timeToLive(uint16_t);

		static SrvDropItem create(Entity entity);
		static SrvDropItem create(uint8_t *buffer);
		static std::unique_ptr<SrvDropItem> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		float x_;
		float y_;
		uint16_t header_;
		uint32_t data_;
		uint16_t id_;
		uint16_t ownerId_;
		uint16_t timeToLive_= 5;
};

}
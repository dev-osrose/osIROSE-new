#include "srv_dropitem.h"
#include "throwassert.h"

namespace RoseCommon {

SrvDropItem::SrvDropItem() : CRosePacket(ePacketType::PAKWC_DROP_ITEM) {}

SrvDropItem::SrvDropItem(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKWC_DROP_ITEM, "Not the right packet: " << to_underlying(get_type()));
	reader.get_float(x_);
	reader.get_float(y_);
	reader.get_uint16_t(header_);
	reader.get_uint32_t(data_);
	reader.get_uint16_t(id_);
	reader.get_uint16_t(ownerId_);
	reader.get_uint16_t(timeToLive_);
}

SrvDropItem::SrvDropItem(float x, float y, uint16_t header, uint32_t data, uint16_t id, uint16_t ownerId) : CRosePacket(ePacketType::PAKWC_DROP_ITEM), x_(x), y_(y), header_(header), data_(data), id_(id), ownerId_(ownerId) {
}

float SrvDropItem::x() const { return x_; }

SrvDropItem& SrvDropItem::set_x(float data) { x_ = data; return *this; }

float SrvDropItem::y() const { return y_; }

SrvDropItem& SrvDropItem::set_y(float data) { y_ = data; return *this; }

uint16_t SrvDropItem::header() const { return header_; }

SrvDropItem& SrvDropItem::set_header(uint16_t data) { header_ = data; return *this; }

uint32_t SrvDropItem::data() const { return data_; }

SrvDropItem& SrvDropItem::set_data(uint32_t data) { data_ = data; return *this; }

uint16_t SrvDropItem::id() const { return id_; }

SrvDropItem& SrvDropItem::set_id(uint16_t data) { id_ = data; return *this; }

uint16_t SrvDropItem::ownerId() const { return ownerId_; }

SrvDropItem& SrvDropItem::set_ownerId(uint16_t data) { ownerId_ = data; return *this; }

uint16_t SrvDropItem::timeToLive() const { return timeToLive_; }

SrvDropItem& SrvDropItem::set_timeToLive(uint16_t data) { timeToLive_ = data; return *this; }


void SrvDropItem::pack(CRoseWriter& writer) const {
	writer.set_float(x_);
	writer.set_float(y_);
	writer.set_uint16_t(header_);
	writer.set_uint32_t(data_);
	writer.set_uint16_t(id_);
	writer.set_uint16_t(ownerId_);
	writer.set_uint16_t(timeToLive_);
}

uint16_t SrvDropItem::get_size() const {
	uint16_t size = 0;
	size += sizeof(x_);
	size += sizeof(y_);
	size += sizeof(header_);
	size += sizeof(data_);
	size += sizeof(id_);
	size += sizeof(ownerId_);
	size += sizeof(timeToLive_);
	return size;
}


SrvDropItem SrvDropItem::create(Entity entity) {
	const auto entity_basicinfo = entity.component<BasicInfo>();
	const auto entity_rosecommon_item = entity.component<RoseCommon::Item>();
	const auto entity_position = entity.component<Position>();

	return SrvDropItem(entity_position->x_, entity_position->y_, entity_rosecommon_item->getHeader(), entity_rosecommon_item->getData(), entity_basicinfo->id_, entity_basicinfo->ownerId_);
}

SrvDropItem SrvDropItem::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvDropItem(reader);
}

}
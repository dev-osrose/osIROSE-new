#include "srv_adjustposition.h"
#include "throwassert.h"

namespace RoseCommon {

SrvAdjustPosition::SrvAdjustPosition() : CRosePacket(ePacketType::PAKWC_ADJUST_POSITION) {}

SrvAdjustPosition::SrvAdjustPosition(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKWC_ADJUST_POSITION, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint16_t(id_);
	reader.get_float(x_);
	reader.get_float(y_);
	reader.get_float(z_);
}

SrvAdjustPosition::SrvAdjustPosition(uint16_t id, float x, float y, float z) : CRosePacket(ePacketType::PAKWC_ADJUST_POSITION), id_(id), x_(x), y_(y), z_(z) {
}

uint16_t SrvAdjustPosition::id() const { return id_; }

SrvAdjustPosition& SrvAdjustPosition::set_id(uint16_t data) { id_ = data; return *this; }

float SrvAdjustPosition::x() const { return x_; }

SrvAdjustPosition& SrvAdjustPosition::set_x(float data) { x_ = data; return *this; }

float SrvAdjustPosition::y() const { return y_; }

SrvAdjustPosition& SrvAdjustPosition::set_y(float data) { y_ = data; return *this; }

float SrvAdjustPosition::z() const { return z_; }

SrvAdjustPosition& SrvAdjustPosition::set_z(float data) { z_ = data; return *this; }


void SrvAdjustPosition::pack(CRoseWriter& writer) const {
	writer.set_uint16_t(id_);
	writer.set_float(x_);
	writer.set_float(y_);
	writer.set_float(z_);
}

uint16_t SrvAdjustPosition::get_size() const {
	uint16_t size = 0;
	size += sizeof(id_);
	size += sizeof(x_);
	size += sizeof(y_);
	size += sizeof(z_);
	return size;
}


SrvAdjustPosition SrvAdjustPosition::create(Entity entity) {
	const auto entity_position = entity.component<Position>();
	const auto entity_basicinfo = entity.component<BasicInfo>();

	return SrvAdjustPosition(entity_basicinfo->id_, entity_position->x_, entity_position->y_, entity_position->z_);
}

SrvAdjustPosition SrvAdjustPosition::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvAdjustPosition(reader);
}

}
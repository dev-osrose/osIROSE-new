#include "srv_stop.h"
#include "throwassert.h"

namespace RoseCommon {

SrvStop::SrvStop() : CRosePacket(ePacketType::PAKWC_STOP) {}

SrvStop::SrvStop(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKWC_STOP, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint16_t(id_);
	reader.get_float(x_);
	reader.get_float(y_);
	reader.get_uint16_t(z_);
}

SrvStop::SrvStop(uint16_t id, float x, float y, uint16_t z) : CRosePacket(ePacketType::PAKWC_STOP), id_(id), x_(x), y_(y), z_(z) {
}

uint16_t SrvStop::id() const { return id_; }

SrvStop& SrvStop::set_id(uint16_t data) { id_ = data; return *this; }

float SrvStop::x() const { return x_; }

SrvStop& SrvStop::set_x(float data) { x_ = data; return *this; }

float SrvStop::y() const { return y_; }

SrvStop& SrvStop::set_y(float data) { y_ = data; return *this; }

uint16_t SrvStop::z() const { return z_; }

SrvStop& SrvStop::set_z(uint16_t data) { z_ = data; return *this; }


void SrvStop::pack(CRoseWriter& writer) const {
	writer.set_uint16_t(id_);
	writer.set_float(x_);
	writer.set_float(y_);
	writer.set_uint16_t(z_);
}

uint16_t SrvStop::get_size() const {
	uint16_t size = 0;
	size += sizeof(id_);
	size += sizeof(x_);
	size += sizeof(y_);
	size += sizeof(z_);
	return size;
}


SrvStop SrvStop::create(Entity entity) {
	const auto entity_basicinfo = entity.component<BasicInfo>();
	const auto entity_position = entity.component<Position>();

	return SrvStop(entity_basicinfo->id_, entity_position->x_, entity_position->y_, entity_position->z_);
}

SrvStop SrvStop::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvStop(reader);
}

}
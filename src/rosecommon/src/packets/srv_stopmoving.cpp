#include "srv_stopmoving.h"
#include "throwassert.h"

namespace RoseCommon {

SrvStopMoving::SrvStopMoving() : CRosePacket(ePacketType::PAKWC_STOP_MOVING) {}

SrvStopMoving::SrvStopMoving(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKWC_STOP_MOVING, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint16_t(id_);
	reader.get_float(x_);
	reader.get_float(y_);
	reader.get_uint16_t(z_);
}

SrvStopMoving::SrvStopMoving(uint16_t id, float x, float y, uint16_t z) : CRosePacket(ePacketType::PAKWC_STOP_MOVING), id_(id), x_(x), y_(y), z_(z) {
}

uint16_t SrvStopMoving::id() const { return id_; }

SrvStopMoving& SrvStopMoving::set_id(uint16_t data) { id_ = data; return *this; }

float SrvStopMoving::x() const { return x_; }

SrvStopMoving& SrvStopMoving::set_x(float data) { x_ = data; return *this; }

float SrvStopMoving::y() const { return y_; }

SrvStopMoving& SrvStopMoving::set_y(float data) { y_ = data; return *this; }

uint16_t SrvStopMoving::z() const { return z_; }

SrvStopMoving& SrvStopMoving::set_z(uint16_t data) { z_ = data; return *this; }


void SrvStopMoving::pack(CRoseBasePolicy& writer) const {
	writer.set_uint16_t(id_);
	writer.set_float(x_);
	writer.set_float(y_);
	writer.set_uint16_t(z_);
}

SrvStopMoving SrvStopMoving::create(Entity entity) {
	const auto entity_position = entity.component<Position>();
	const auto entity_basicinfo = entity.component<BasicInfo>();

	return SrvStopMoving(entity_basicinfo->id_, entity_position->x_, entity_position->y_, entity_position->z_);
}

SrvStopMoving SrvStopMoving::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvStopMoving(reader);
}
std::unique_ptr<SrvStopMoving> SrvStopMoving::allocate(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return std::make_unique<SrvStopMoving>(reader);
}

}
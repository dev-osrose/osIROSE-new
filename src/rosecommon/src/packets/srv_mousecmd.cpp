#include "srv_mousecmd.h"
#include "throwassert.h"

namespace RoseCommon {

SrvMouseCmd::SrvMouseCmd() : CRosePacket(ePacketType::PAKWC_MOUSE_CMD) {}

SrvMouseCmd::SrvMouseCmd(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKWC_MOUSE_CMD, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint16_t(id_);
	reader.get_uint16_t(targetId_);
	reader.get_uint16_t(dist_);
	reader.get_float(x_);
	reader.get_float(y_);
	reader.get_uint16_t(z_);
}

SrvMouseCmd::SrvMouseCmd(uint16_t id, uint16_t targetId, uint16_t dist, float x, float y, uint16_t z) : CRosePacket(ePacketType::PAKWC_MOUSE_CMD), id_(id), targetId_(targetId), dist_(dist), x_(x), y_(y), z_(z) {
}

uint16_t SrvMouseCmd::id() const { return id_; }

SrvMouseCmd& SrvMouseCmd::set_id(uint16_t data) { id_ = data; return *this; }

uint16_t SrvMouseCmd::targetId() const { return targetId_; }

SrvMouseCmd& SrvMouseCmd::set_targetId(uint16_t data) { targetId_ = data; return *this; }

uint16_t SrvMouseCmd::dist() const { return dist_; }

SrvMouseCmd& SrvMouseCmd::set_dist(uint16_t data) { dist_ = data; return *this; }

float SrvMouseCmd::x() const { return x_; }

SrvMouseCmd& SrvMouseCmd::set_x(float data) { x_ = data; return *this; }

float SrvMouseCmd::y() const { return y_; }

SrvMouseCmd& SrvMouseCmd::set_y(float data) { y_ = data; return *this; }

uint16_t SrvMouseCmd::z() const { return z_; }

SrvMouseCmd& SrvMouseCmd::set_z(uint16_t data) { z_ = data; return *this; }


void SrvMouseCmd::pack(CRoseBasePolicy& writer) const {
	writer.set_uint16_t(id_);
	writer.set_uint16_t(targetId_);
	writer.set_uint16_t(dist_);
	writer.set_float(x_);
	writer.set_float(y_);
	writer.set_uint16_t(z_);
}

SrvMouseCmd SrvMouseCmd::create(Entity entity) {
	const auto entity_position = entity.component<Position>();
	const auto entity_destination = entity.component<Destination>();
	const auto entity_basicinfo = entity.component<BasicInfo>();

	return SrvMouseCmd(entity_basicinfo->id_, entity_basicinfo->targetId_, entity_destination->dist_, entity_destination->x_, entity_destination->y_, entity_position->z_);
}

SrvMouseCmd SrvMouseCmd::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvMouseCmd(reader);
}
std::unique_ptr<SrvMouseCmd> SrvMouseCmd::allocate(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return std::make_unique<SrvMouseCmd>(reader);
}

}
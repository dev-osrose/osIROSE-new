#include "srv_teleportreply.h"
#include "throwassert.h"

namespace RoseCommon {

SrvTeleportReply::SrvTeleportReply() : CRosePacket(ePacketType::PAKWC_TELEPORT_REPLY) {}

SrvTeleportReply::SrvTeleportReply(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKWC_TELEPORT_REPLY, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint16_t(id_);
	reader.get_uint16_t(map_);
	reader.get_float(x_);
	reader.get_float(y_);
	reader.get_uint8_t(moveMode_);
	reader.get_uint8_t(rideMode_);
}

SrvTeleportReply::SrvTeleportReply(uint16_t id, uint16_t map, float x, float y, uint8_t moveMode) : CRosePacket(ePacketType::PAKWC_TELEPORT_REPLY), id_(id), map_(map), x_(x), y_(y), moveMode_(moveMode) {
}

uint16_t SrvTeleportReply::id() const { return id_; }

SrvTeleportReply& SrvTeleportReply::set_id(uint16_t data) { id_ = data; return *this; }

uint16_t SrvTeleportReply::map() const { return map_; }

SrvTeleportReply& SrvTeleportReply::set_map(uint16_t data) { map_ = data; return *this; }

float SrvTeleportReply::x() const { return x_; }

SrvTeleportReply& SrvTeleportReply::set_x(float data) { x_ = data; return *this; }

float SrvTeleportReply::y() const { return y_; }

SrvTeleportReply& SrvTeleportReply::set_y(float data) { y_ = data; return *this; }

uint8_t SrvTeleportReply::moveMode() const { return moveMode_; }

SrvTeleportReply& SrvTeleportReply::set_moveMode(uint8_t data) { moveMode_ = data; return *this; }

uint8_t SrvTeleportReply::rideMode() const { return rideMode_; }

SrvTeleportReply& SrvTeleportReply::set_rideMode(uint8_t data) { rideMode_ = data; return *this; }


void SrvTeleportReply::pack(CRoseWriter& writer) const {
	writer.set_uint16_t(id_);
	writer.set_uint16_t(map_);
	writer.set_float(x_);
	writer.set_float(y_);
	writer.set_uint8_t(moveMode_);
	writer.set_uint8_t(rideMode_);
}

uint16_t SrvTeleportReply::get_size() const {
	uint16_t size = 0;
	size += sizeof(id_);
	size += sizeof(map_);
	size += sizeof(x_);
	size += sizeof(y_);
	size += sizeof(moveMode_);
	size += sizeof(rideMode_);
	return size;
}


SrvTeleportReply SrvTeleportReply::create(Entity entity) {
	const auto entity_advancedinfo = entity.component<AdvancedInfo>();
	const auto entity_position = entity.component<Position>();
	const auto entity_basicinfo = entity.component<BasicInfo>();

	return SrvTeleportReply(entity_basicinfo->id_, entity_position->map_, entity_position->x_, entity_position->y_, entity_advancedinfo->moveMode_);
}

SrvTeleportReply SrvTeleportReply::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvTeleportReply(reader);
}

}
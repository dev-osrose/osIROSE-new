#include "srv_attack.h"
#include "throwassert.h"

namespace RoseCommon {

SrvAttack::SrvAttack() : CRosePacket(ePacketType::PAKWC_ATTACK) {}

SrvAttack::SrvAttack(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKWC_ATTACK, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint16_t(attackerId_);
	reader.get_uint16_t(defenderId_);
	reader.get_uint16_t(dist_);
	reader.get_float(x_);
	reader.get_float(y_);
	reader.get_uint16_t(z_);
}

SrvAttack::SrvAttack(uint16_t attackerId, uint16_t defenderId, uint16_t dist, float x, float y, uint16_t z) : CRosePacket(ePacketType::PAKWC_ATTACK), attackerId_(attackerId), defenderId_(defenderId), dist_(dist), x_(x), y_(y), z_(z) {
}

uint16_t SrvAttack::attackerId() const { return attackerId_; }

SrvAttack& SrvAttack::set_attackerId(uint16_t data) { attackerId_ = data; return *this; }

uint16_t SrvAttack::defenderId() const { return defenderId_; }

SrvAttack& SrvAttack::set_defenderId(uint16_t data) { defenderId_ = data; return *this; }

uint16_t SrvAttack::dist() const { return dist_; }

SrvAttack& SrvAttack::set_dist(uint16_t data) { dist_ = data; return *this; }

float SrvAttack::x() const { return x_; }

SrvAttack& SrvAttack::set_x(float data) { x_ = data; return *this; }

float SrvAttack::y() const { return y_; }

SrvAttack& SrvAttack::set_y(float data) { y_ = data; return *this; }

uint16_t SrvAttack::z() const { return z_; }

SrvAttack& SrvAttack::set_z(uint16_t data) { z_ = data; return *this; }


void SrvAttack::pack(CRoseBasePolicy& writer) const {
	writer.set_uint16_t(attackerId_);
	writer.set_uint16_t(defenderId_);
	writer.set_uint16_t(dist_);
	writer.set_float(x_);
	writer.set_float(y_);
	writer.set_uint16_t(z_);
}

SrvAttack SrvAttack::create(Entity entityA, Entity entityD) {
	const auto entitya_destination = entityA.component<Destination>();
	const auto entitya_basicinfo = entityA.component<BasicInfo>();
	const auto entityd_basicinfo = entityD.component<BasicInfo>();

	return SrvAttack(entitya_basicinfo->id_, entityd_basicinfo->id_, entitya_destination->dist_, entitya_destination->x_, entitya_destination->y_, entitya_destination->z_);
}

SrvAttack SrvAttack::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvAttack(reader);
}
std::unique_ptr<SrvAttack> SrvAttack::allocate(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return std::make_unique<SrvAttack>(reader);
}

}
#include "srv_damage.h"
#include "throwassert.h"

namespace RoseCommon {

SrvDamage::SrvDamage() : CRosePacket(ePacketType::PAKWC_DAMAGE) {}

SrvDamage::SrvDamage(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKWC_DAMAGE, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint16_t(idA_);
	reader.get_uint16_t(idD_);
	reader.get_uint32_t(damage_);
	reader.get_uint32_t(action_);
}

SrvDamage::SrvDamage(uint16_t idA, uint16_t idD, uint32_t damage, uint32_t action) : CRosePacket(ePacketType::PAKWC_DAMAGE), idA_(idA), idD_(idD), damage_(damage), action_(action) {
}

uint16_t SrvDamage::idA() const { return idA_; }

SrvDamage& SrvDamage::set_idA(uint16_t data) { idA_ = data; return *this; }

uint16_t SrvDamage::idD() const { return idD_; }

SrvDamage& SrvDamage::set_idD(uint16_t data) { idD_ = data; return *this; }

uint32_t SrvDamage::damage() const { return damage_; }

SrvDamage& SrvDamage::set_damage(uint32_t data) { damage_ = data; return *this; }

uint32_t SrvDamage::action() const { return action_; }

SrvDamage& SrvDamage::set_action(uint32_t data) { action_ = data; return *this; }


void SrvDamage::pack(CRoseBasePolicy& writer) const {
	writer.set_uint16_t(idA_);
	writer.set_uint16_t(idD_);
	writer.set_uint32_t(damage_);
	writer.set_uint32_t(action_);
}

SrvDamage SrvDamage::create(Entity entityA, Entity entityD, uint32_t damage, uint32_t action) {
	const auto entitya_basicinfo = entityA.component<BasicInfo>();
	const auto entityd_basicinfo = entityD.component<BasicInfo>();

	return SrvDamage(entitya_basicinfo->id_, entityd_basicinfo->id_, damage, action);
}

SrvDamage SrvDamage::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvDamage(reader);
}
std::unique_ptr<SrvDamage> SrvDamage::allocate(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return std::make_unique<SrvDamage>(reader);
}

}
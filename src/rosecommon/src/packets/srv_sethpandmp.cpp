#include "srv_sethpandmp.h"
#include "throwassert.h"

namespace RoseCommon {

SrvSetHpAndMp::SrvSetHpAndMp() : CRosePacket(ePacketType::PAKWC_SET_HP_AND_MP) {}

SrvSetHpAndMp::SrvSetHpAndMp(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKWC_SET_HP_AND_MP, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint16_t(id_);
	reader.get_uint16_t(hp_);
	reader.get_uint16_t(mp_);
}

SrvSetHpAndMp::SrvSetHpAndMp(uint16_t id, uint16_t hp, uint16_t mp) : CRosePacket(ePacketType::PAKWC_SET_HP_AND_MP), id_(id), hp_(hp), mp_(mp) {
}

uint16_t SrvSetHpAndMp::id() const { return id_; }

SrvSetHpAndMp& SrvSetHpAndMp::set_id(uint16_t data) { id_ = data; return *this; }

uint16_t SrvSetHpAndMp::hp() const { return hp_; }

SrvSetHpAndMp& SrvSetHpAndMp::set_hp(uint16_t data) { hp_ = data; return *this; }

uint16_t SrvSetHpAndMp::mp() const { return mp_; }

SrvSetHpAndMp& SrvSetHpAndMp::set_mp(uint16_t data) { mp_ = data; return *this; }


void SrvSetHpAndMp::pack(CRoseBasePolicy& writer) const {
	writer.set_uint16_t(id_);
	writer.set_uint16_t(hp_);
	writer.set_uint16_t(mp_);
}

SrvSetHpAndMp SrvSetHpAndMp::create(Entity entity, uint16_t hp, uint16_t mp) {
	const auto entity_basicinfo = entity.component<BasicInfo>();

	return SrvSetHpAndMp(entity_basicinfo->id_, hp, mp);
}

SrvSetHpAndMp SrvSetHpAndMp::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvSetHpAndMp(reader);
}
std::unique_ptr<SrvSetHpAndMp> SrvSetHpAndMp::allocate(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return std::make_unique<SrvSetHpAndMp>(reader);
}

}
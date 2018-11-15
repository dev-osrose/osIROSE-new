#include "srv_hpreply.h"
#include "throwassert.h"

namespace RoseCommon {

SrvHpReply::SrvHpReply() : CRosePacket(ePacketType::PAKWC_HP_REPLY) {}

SrvHpReply::SrvHpReply(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKWC_HP_REPLY, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint16_t(id_);
	reader.get_int32_t(hp_);
}

SrvHpReply::SrvHpReply(uint16_t id, int32_t hp) : CRosePacket(ePacketType::PAKWC_HP_REPLY), id_(id), hp_(hp) {
}

uint16_t SrvHpReply::id() const { return id_; }

SrvHpReply& SrvHpReply::set_id(uint16_t data) { id_ = data; return *this; }

int32_t SrvHpReply::hp() const { return hp_; }

SrvHpReply& SrvHpReply::set_hp(int32_t data) { hp_ = data; return *this; }


void SrvHpReply::pack(CRoseWriter& writer) const {
	writer.set_uint16_t(id_);
	writer.set_int32_t(hp_);
}

uint16_t SrvHpReply::get_size() const {
	uint16_t size = 0;
	size += sizeof(id_);
	size += sizeof(hp_);
	return size;
}


SrvHpReply SrvHpReply::create(Entity entity) {
	const auto entity_basicinfo = entity.component<BasicInfo>();
	const auto entity_advancedinfo = entity.component<AdvancedInfo>();

	return SrvHpReply(entity_basicinfo->id_, entity_advancedinfo->hp_);
}

SrvHpReply SrvHpReply::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvHpReply(reader);
}

}
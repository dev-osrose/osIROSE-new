#include "srv_removeobject.h"
#include "throwassert.h"

namespace RoseCommon {

SrvRemoveObject::SrvRemoveObject() : CRosePacket(ePacketType::PAKWC_REMOVE_OBJECT) {}

SrvRemoveObject::SrvRemoveObject(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKWC_REMOVE_OBJECT, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint16_t(id_);
}

SrvRemoveObject::SrvRemoveObject(uint16_t id) : CRosePacket(ePacketType::PAKWC_REMOVE_OBJECT), id_(id) {
}

uint16_t SrvRemoveObject::id() const { return id_; }

SrvRemoveObject& SrvRemoveObject::set_id(uint16_t data) { id_ = data; return *this; }


void SrvRemoveObject::pack(CRoseBasePolicy& writer) const {
	writer.set_uint16_t(id_);
}

SrvRemoveObject SrvRemoveObject::create(Entity entity) {
	const auto entity_basicinfo = entity.component<BasicInfo>();

	return SrvRemoveObject(entity_basicinfo->id_);
}

SrvRemoveObject SrvRemoveObject::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvRemoveObject(reader);
}
std::unique_ptr<SrvRemoveObject> SrvRemoveObject::allocate(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return std::make_unique<SrvRemoveObject>(reader);
}

}
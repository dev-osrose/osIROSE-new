#include "srv_setmoney.h"
#include "throwassert.h"

namespace RoseCommon {

SrvSetMoney::SrvSetMoney() : CRosePacket(ePacketType::PAKWC_SET_MONEY) {}

SrvSetMoney::SrvSetMoney(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKWC_SET_MONEY, "Not the right packet: " << to_underlying(get_type()));
	reader.get_int64_t(zuly_);
}

SrvSetMoney::SrvSetMoney(int64_t zuly) : CRosePacket(ePacketType::PAKWC_SET_MONEY), zuly_(zuly) {
}

int64_t SrvSetMoney::zuly() const { return zuly_; }

SrvSetMoney& SrvSetMoney::set_zuly(int64_t data) { zuly_ = data; return *this; }


void SrvSetMoney::pack(CRoseBasePolicy& writer) const {
	writer.set_int64_t(zuly_);
}

SrvSetMoney SrvSetMoney::create(Entity entity) {
	const auto entity_advancedinfo = entity.component<AdvancedInfo>();

	return SrvSetMoney(entity_advancedinfo->zuly_);
}

SrvSetMoney SrvSetMoney::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvSetMoney(reader);
}
std::unique_ptr<SrvSetMoney> SrvSetMoney::allocate(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return std::make_unique<SrvSetMoney>(reader);
}

}
#include "srv_setmoney.h"
#include "throwassert.h"

namespace RoseCommon {

SrvSetMoney::SrvSetMoney() : CRosePacket(ePacketType::PAKWC_SET_MONEY) {}

SrvSetMoney::SrvSetMoney(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	throw_assert(type() == ePacketType::PAKWC_SET_MONEY, "Not the right packet: " << to_underlying(type()));
	auto advancedInfo = entity_.component<AdvancedInfo>();

	*this >> advancedInfo->zuly_;

}

SrvSetMoney::SrvSetMoney(Entity entity) : CRosePacket(ePacketType::PAKWC_SET_MONEY), entity_(entity) {}

Entity SrvSetMoney::entity() const {
	return entity_;
}


void SrvSetMoney::pack() {
	auto advancedInfo = entity_.component<AdvancedInfo>();

	*this << advancedInfo->zuly_;

}

}
#include "srv_setmoney.h"

namespace RoseCommon {

SrvSetMoney::SrvSetMoney() : CRosePacket(ePacketType::PAKWC_SET_MONEY) {}

SrvSetMoney::SrvSetMoney(Entity entity) : CRosePacket(ePacketType::PAKWC_SET_MONEY), entity_(entity) {}

Entity SrvSetMoney::entity() const {
	return entity_;
}


void SrvSetMoney::pack() {
	auto advancedInfo = entity_.component<AdvancedInfo>();

	*this << advancedInfo->zuly_;

}

}
#include "srv_adjustposition.h"
#include "throwassert.h"

namespace RoseCommon {

SrvAdjustPosition::SrvAdjustPosition() : CRosePacket(ePacketType::PAKWC_ADJUST_POSITION) {}

SrvAdjustPosition::SrvAdjustPosition(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	throw_assert(type() == ePacketType::PAKWC_ADJUST_POSITION, "Not the right packet: " << to_underlying(type()));
	auto basicInfo = entity_.component<BasicInfo>();
	auto position = entity_.component<Position>();

	*this >> basicInfo->id_;
	*this >> position->x_;
	*this >> position->y_;
	*this >> position->z_;

}

SrvAdjustPosition::SrvAdjustPosition(Entity entity) : CRosePacket(ePacketType::PAKWC_ADJUST_POSITION), entity_(entity) {}

Entity SrvAdjustPosition::entity() const {
	return entity_;
}


void SrvAdjustPosition::pack() {
	auto basicInfo = entity_.component<BasicInfo>();
	auto position = entity_.component<Position>();

	*this << basicInfo->id_;
	*this << position->x_;
	*this << position->y_;
	*this << position->z_;

}

}
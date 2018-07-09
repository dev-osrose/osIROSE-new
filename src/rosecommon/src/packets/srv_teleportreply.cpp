#include "srv_teleportreply.h"
#include "throwassert.h"

namespace RoseCommon {

SrvTeleportReply::SrvTeleportReply() : CRosePacket(ePacketType::PAKWC_TELEPORT_REPLY) {}

SrvTeleportReply::SrvTeleportReply(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	throw_assert(type() == ePacketType::PAKWC_TELEPORT_REPLY, "Not the right packet: " << to_underlying(type()));
	auto basicInfo = entity_.component<BasicInfo>();
	auto advancedInfo = entity_.component<AdvancedInfo>();
	auto position = entity_.component<Position>();

	*this >> basicInfo->id_;
	*this >> position->map_;
	*this >> position->x_;
	*this >> position->y_;
	*this >> advancedInfo->moveMode_;
	uint8_t rideMode;
	*this >> rideMode;

}

SrvTeleportReply::SrvTeleportReply(Entity entity) : CRosePacket(ePacketType::PAKWC_TELEPORT_REPLY), entity_(entity) {}

Entity SrvTeleportReply::entity() const {
	return entity_;
}


void SrvTeleportReply::pack() {
	auto basicInfo = entity_.component<BasicInfo>();
	auto advancedInfo = entity_.component<AdvancedInfo>();
	auto position = entity_.component<Position>();

	*this << basicInfo->id_;
	*this << position->map_;
	*this << position->x_;
	*this << position->y_;
	*this << advancedInfo->moveMode_;
	*this << (uint8_t)0; // rideMode

}

}

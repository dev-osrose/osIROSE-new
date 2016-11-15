#include "cli_changemapreq.h"

namespace RoseCommon {

CliChangeMapReq::CliChangeMapReq() : CRosePacket(ePacketType::PAKCS_CHANGE_MAP_REQ) {}

CliChangeMapReq::CliChangeMapReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	if (type() != ePacketType::PAKCS_CHANGE_MAP_REQ)
		throw std::runtime_error("Not the right packet!");
	auto position = entity_.component<Position>();
	auto advancedInfo = entity_.component<AdvancedInfo>();

	*this >> advancedInfo->weightRate_;
	*this >> position->z_;

}

CliChangeMapReq::CliChangeMapReq(Entity entity) : CRosePacket(ePacketType::PAKCS_CHANGE_MAP_REQ), entity_(entity) {}

Entity CliChangeMapReq::entity() const {
	return entity_;
}


void CliChangeMapReq::pack() {
	auto position = entity_.component<Position>();
	auto advancedInfo = entity_.component<AdvancedInfo>();

	*this << advancedInfo->weightRate_;
	*this << position->z_;

}

}
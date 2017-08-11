#include "cli_revivereq.h"
#include "throwassert.h"

namespace RoseCommon {

const RecvPacketFactory::Initializer<uint8_t[MAX_PACKET_SIZE]> CliReviveReq::init = RecvPacketFactory::Initializer<uint8_t[MAX_PACKET_SIZE]>(ePacketType::PAKCS_REVIVE_REQ, &createPacket<CliReviveReq>);

CliReviveReq::CliReviveReq() : CRosePacket(ePacketType::PAKCS_REVIVE_REQ) {}

CliReviveReq::CliReviveReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	throw_assert(type() == ePacketType::PAKCS_REVIVE_REQ, "Not the right packet: " << to_underlying(type()));
	*this >> reviveType_;
}

CliReviveReq::CliReviveReq(ReviveReq::ReviveReq reviveType) : CRosePacket(ePacketType::PAKCS_REVIVE_REQ), reviveType_(reviveType) {}

ReviveReq::ReviveReq CliReviveReq::reviveType() const {
	return reviveType_;
}


void CliReviveReq::pack() {
	*this << reviveType_;
}

}
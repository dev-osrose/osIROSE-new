#include "cli_revivereq.h"
#include "throwassert.h"

namespace RoseCommon {

CliReviveReq::CliReviveReq() : CRosePacket(ePacketType::PAKCS_REVIVE_REQ) {}

CliReviveReq::CliReviveReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
    throw_assert(CRosePacket::type() == ePacketType::PAKCS_REVIVE_REQ, "Not the right packet: " << to_underlying(type()));
	*this >> type_;
}

CliReviveReq::CliReviveReq(CliReviveReq::eType type) : CRosePacket(ePacketType::PAKCS_REVIVE_REQ), type_(type) {}

CliReviveReq::eType CliReviveReq::type() const {
	return type_;
}


void CliReviveReq::pack() {
	*this << type_;
}

}

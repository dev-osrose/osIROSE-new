#include "cli_revivereq.h"

namespace RoseCommon {

CliReviveReq::CliReviveReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	if (type() != ePacketType::PAKCS_REVIVE_REQ)
		throw std::runtime_error("Not the right packet!");
	*this >> type_;

}

CliReviveReq::eType &CliReviveReq::type() {
	return type_;
}

}

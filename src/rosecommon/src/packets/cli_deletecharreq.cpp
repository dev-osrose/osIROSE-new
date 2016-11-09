#include "cli_deletecharreq.h"

namespace RoseCommon {

CliDeleteCharReq::CliDeleteCharReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	if (type() != ePacketType::PAKCS_DELETE_CHAR_REQ)
		throw std::runtime_error("Not the right packet!");
	*this >> charId_;
	*this >> delete_;
	*this >> name_;

}

uint8_t &CliDeleteCharReq::charId() {
	return charId_;
}

bool CliDeleteCharReq::isDelete() const {
	return delete_;
}

std::string &CliDeleteCharReq::name() {
	return name_;
}

}

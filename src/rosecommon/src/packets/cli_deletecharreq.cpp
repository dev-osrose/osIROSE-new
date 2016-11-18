#include "cli_deletecharreq.h"

namespace RoseCommon {

CliDeleteCharReq::CliDeleteCharReq() : CRosePacket(ePacketType::PAKCS_DELETE_CHAR_REQ) {}

CliDeleteCharReq::CliDeleteCharReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	if (type() != ePacketType::PAKCS_DELETE_CHAR_REQ)
		throw std::runtime_error("Not the right packet!");
	*this >> charId_;
	*this >> isDelete_;
	*this >> name_;
}

CliDeleteCharReq::CliDeleteCharReq(uint8_t charId, uint8_t isDelete, const std::string &name) : CRosePacket(ePacketType::PAKCS_DELETE_CHAR_REQ), charId_(charId), isDelete_(isDelete), name_(name) {}

uint8_t CliDeleteCharReq::charId() const {
	return charId_;
}

uint8_t CliDeleteCharReq::isDelete() const {
	return isDelete_;
}

std::string &CliDeleteCharReq::name() {
	return name_;
}

const std::string &CliDeleteCharReq::name() const {
	return name_;
}


void CliDeleteCharReq::pack() {
	*this << charId_;
	*this << isDelete_;
	*this << name_;
}

}
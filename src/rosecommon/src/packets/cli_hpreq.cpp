#include "cli_hpreq.h"
#include "throwassert.h"

namespace RoseCommon {

CliHpReq::CliHpReq() : CRosePacket(ePacketType::PAKCS_HP_REQ) {}

CliHpReq::CliHpReq(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKCS_HP_REQ, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint16_t(targetId_);
}

CliHpReq::CliHpReq(uint16_t targetId) : CRosePacket(ePacketType::PAKCS_HP_REQ), targetId_(targetId) {
}

uint16_t CliHpReq::targetId() const { return targetId_; }

CliHpReq& CliHpReq::set_targetId(uint16_t data) { targetId_ = data; return *this; }


void CliHpReq::pack(CRoseWriter& writer) const {
	writer.set_uint16_t(targetId_);
}

uint16_t CliHpReq::get_size() const {
	uint16_t size = 0;
	size += sizeof(targetId_);
	return size;
}


CliHpReq CliHpReq::create(uint16_t targetId) {


	return CliHpReq(targetId);
}

CliHpReq CliHpReq::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return CliHpReq(reader);
}

}
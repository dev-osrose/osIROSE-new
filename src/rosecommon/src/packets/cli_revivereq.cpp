#include "cli_revivereq.h"
#include "throwassert.h"

namespace RoseCommon {

CliReviveReq::CliReviveReq() : CRosePacket(ePacketType::PAKCS_REVIVE_REQ) {}

CliReviveReq::CliReviveReq(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKCS_REVIVE_REQ, "Not the right packet: " << to_underlying(get_type()));
	{ uint8_t _tmp = uint8_t(); reader.get_uint8_t(_tmp); reviveType_ = static_cast<ReviveReq::ReviveReq>(_tmp); }
}

CliReviveReq::CliReviveReq(ReviveReq::ReviveReq reviveType) : CRosePacket(ePacketType::PAKCS_REVIVE_REQ), reviveType_(reviveType) {
}

ReviveReq::ReviveReq CliReviveReq::reviveType() const { return reviveType_; }

CliReviveReq& CliReviveReq::set_reviveType(ReviveReq::ReviveReq data) { reviveType_ = data; return *this; }


void CliReviveReq::pack(CRoseWriter& writer) const {
	writer.set_uint8_t(reviveType_);
}

uint16_t CliReviveReq::get_size() const {
	uint16_t size = 0;
	size += sizeof(reviveType_);
	return size;
}


CliReviveReq CliReviveReq::create(ReviveReq::ReviveReq reviveType) {


	return CliReviveReq(reviveType);
}

CliReviveReq CliReviveReq::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return CliReviveReq(reader);
}

}
#include "cli_changemapreq.h"
#include "throwassert.h"

namespace RoseCommon {

CliChangeMapReq::CliChangeMapReq() : CRosePacket(ePacketType::PAKCS_CHANGE_MAP_REQ) {}

CliChangeMapReq::CliChangeMapReq(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKCS_CHANGE_MAP_REQ, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint8_t(weightRate_);
	reader.get_uint16_t(z_);
}

CliChangeMapReq::CliChangeMapReq(uint8_t weightRate, uint16_t z) : CRosePacket(ePacketType::PAKCS_CHANGE_MAP_REQ), weightRate_(weightRate), z_(z) {
}

uint8_t CliChangeMapReq::weightRate() const { return weightRate_; }

CliChangeMapReq& CliChangeMapReq::set_weightRate(uint8_t data) { weightRate_ = data; return *this; }

uint16_t CliChangeMapReq::z() const { return z_; }

CliChangeMapReq& CliChangeMapReq::set_z(uint16_t data) { z_ = data; return *this; }


void CliChangeMapReq::pack(CRoseBasePolicy& writer) const {
	writer.set_uint8_t(weightRate_);
	writer.set_uint16_t(z_);
}

CliChangeMapReq CliChangeMapReq::create(uint8_t weightRate, uint16_t z) {


	return CliChangeMapReq(weightRate, z);
}

CliChangeMapReq CliChangeMapReq::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return CliChangeMapReq(reader);
}
std::unique_ptr<CliChangeMapReq> CliChangeMapReq::allocate(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return std::make_unique<CliChangeMapReq>(reader);
}

}
#include "cli_changemapreq.h"
#include "throwassert.h"

namespace RoseCommon {

const RecvPacketFactory::Initializer<uint8_t*> CliChangeMapReq::init = RecvPacketFactory::Initializer<uint8_t*>(ePacketType::PAKCS_CHANGE_MAP_REQ, &createPacket<CliChangeMapReq>);

CliChangeMapReq::CliChangeMapReq() : CRosePacket(ePacketType::PAKCS_CHANGE_MAP_REQ) {}

CliChangeMapReq::CliChangeMapReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	throw_assert(type() == ePacketType::PAKCS_CHANGE_MAP_REQ, "Not the right packet: " << to_underlying(type()));
	weightRate_ = read_uint8();
	z_ = read_uint16();
}

CliChangeMapReq::CliChangeMapReq(uint8_t weightRate, uint16_t z) : CRosePacket(ePacketType::PAKCS_CHANGE_MAP_REQ), weightRate_(weightRate), z_(z) {}

uint8_t CliChangeMapReq::weightRate() const {
	return weightRate_;
}

uint16_t CliChangeMapReq::z() const {
	return z_;
}

void CliChangeMapReq::weightRate(uint8_t rate) {
	weightRate_ = rate;
}

void CliChangeMapReq::z(uint16_t z) {
	z_ = z;
}

void CliChangeMapReq::pack() {
	write_uint8(weightRate_);
	write_uint16(z_);
}

}

#include "cli_selectcharreq.h"
#include "throwassert.h"

namespace RoseCommon {

CliSelectCharReq::CliSelectCharReq() : CRosePacket(ePacketType::PAKCS_SELECT_CHAR_REQ) {}

CliSelectCharReq::CliSelectCharReq(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKCS_SELECT_CHAR_REQ, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint8_t(charId_);
	reader.get_uint8_t(runMode_);
	reader.get_uint8_t(rideMode_);
	reader.get_string(name_);
}

CliSelectCharReq::CliSelectCharReq(uint8_t charId, uint8_t runMode, uint8_t rideMode, const std::string& name) : CRosePacket(ePacketType::PAKCS_SELECT_CHAR_REQ), charId_(charId), runMode_(runMode), rideMode_(rideMode), name_(name) {
}

uint8_t CliSelectCharReq::charId() const { return charId_; }

CliSelectCharReq& CliSelectCharReq::set_charId(uint8_t data) { charId_ = data; return *this; }

uint8_t CliSelectCharReq::runMode() const { return runMode_; }

CliSelectCharReq& CliSelectCharReq::set_runMode(uint8_t data) { runMode_ = data; return *this; }

uint8_t CliSelectCharReq::rideMode() const { return rideMode_; }

CliSelectCharReq& CliSelectCharReq::set_rideMode(uint8_t data) { rideMode_ = data; return *this; }

const std::string& CliSelectCharReq::name() const { return name_; }

CliSelectCharReq& CliSelectCharReq::set_name(const std::string& data) { name_ = data; return *this; }


void CliSelectCharReq::pack(CRoseWriter& writer) const {
	writer.set_uint8_t(charId_);
	writer.set_uint8_t(runMode_);
	writer.set_uint8_t(rideMode_);
	writer.set_string(name_);
}

uint16_t CliSelectCharReq::get_size() const {
	uint16_t size = 0;
	size += sizeof(charId_);
	size += sizeof(runMode_);
	size += sizeof(rideMode_);
	size += sizeof(char) * (name_.size() + 1);
	return size;
}


CliSelectCharReq CliSelectCharReq::create(uint8_t charId, uint8_t runMode, uint8_t rideMode, const std::string& name) {


	return CliSelectCharReq(charId, runMode, rideMode, name);
}

CliSelectCharReq CliSelectCharReq::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return CliSelectCharReq(reader);
}

}
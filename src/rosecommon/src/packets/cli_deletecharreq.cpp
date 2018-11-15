#include "cli_deletecharreq.h"
#include "throwassert.h"

namespace RoseCommon {

CliDeleteCharReq::CliDeleteCharReq() : CRosePacket(ePacketType::PAKCS_DELETE_CHAR_REQ) {}

CliDeleteCharReq::CliDeleteCharReq(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKCS_DELETE_CHAR_REQ, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint8_t(charId_);
	reader.get_uint8_t(isDelete_);
	reader.get_string(name_);
}

CliDeleteCharReq::CliDeleteCharReq(uint8_t charId, uint8_t isDelete, const std::string& name) : CRosePacket(ePacketType::PAKCS_DELETE_CHAR_REQ), charId_(charId), isDelete_(isDelete), name_(name) {
}

uint8_t CliDeleteCharReq::charId() const { return charId_; }

CliDeleteCharReq& CliDeleteCharReq::set_charId(uint8_t data) { charId_ = data; return *this; }

uint8_t CliDeleteCharReq::isDelete() const { return isDelete_; }

CliDeleteCharReq& CliDeleteCharReq::set_isDelete(uint8_t data) { isDelete_ = data; return *this; }

const std::string& CliDeleteCharReq::name() const { return name_; }

CliDeleteCharReq& CliDeleteCharReq::set_name(const std::string& data) { name_ = data; return *this; }


void CliDeleteCharReq::pack(CRoseWriter& writer) const {
	writer.set_uint8_t(charId_);
	writer.set_uint8_t(isDelete_);
	writer.set_string(name_);
}

uint16_t CliDeleteCharReq::get_size() const {
	uint16_t size = 0;
	size += sizeof(charId_);
	size += sizeof(isDelete_);
	size += sizeof(char) * (name_.size() + 1);
	return size;
}


CliDeleteCharReq CliDeleteCharReq::create(uint8_t charId, uint8_t isDelete, const std::string& name) {


	return CliDeleteCharReq(charId, isDelete, name);
}

CliDeleteCharReq CliDeleteCharReq::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return CliDeleteCharReq(reader);
}

}
#include "srv_createcharreply.h"
#include "throwassert.h"

namespace RoseCommon {

SrvCreateCharReply::SrvCreateCharReply() : CRosePacket(ePacketType::PAKCC_CREATE_CHAR_REPLY) {}

SrvCreateCharReply::SrvCreateCharReply(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKCC_CREATE_CHAR_REPLY, "Not the right packet: " << to_underlying(get_type()));
	{ uint8_t _tmp = uint8_t(); reader.get_uint8_t(_tmp); result_ = static_cast<CreateCharReply::Result>(_tmp); }
	reader.get_uint8_t(platinium_);
}

SrvCreateCharReply::SrvCreateCharReply(CreateCharReply::Result result) : CRosePacket(ePacketType::PAKCC_CREATE_CHAR_REPLY), result_(result) {
}

CreateCharReply::Result SrvCreateCharReply::result() const { return result_; }

SrvCreateCharReply& SrvCreateCharReply::set_result(CreateCharReply::Result data) { result_ = data; return *this; }

uint8_t SrvCreateCharReply::platinium() const { return platinium_; }

SrvCreateCharReply& SrvCreateCharReply::set_platinium(uint8_t data) { platinium_ = data; return *this; }


void SrvCreateCharReply::pack(CRoseWriter& writer) const {
	writer.set_uint8_t(result_);
	writer.set_uint8_t(platinium_);
}

uint16_t SrvCreateCharReply::get_size() const {
	uint16_t size = 0;
	size += sizeof(result_);
	size += sizeof(platinium_);
	return size;
}


SrvCreateCharReply SrvCreateCharReply::create(CreateCharReply::Result result) {


	return SrvCreateCharReply(result);
}

SrvCreateCharReply SrvCreateCharReply::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvCreateCharReply(reader);
}

}
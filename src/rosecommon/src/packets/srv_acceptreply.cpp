#include "srv_acceptreply.h"
#include "throwassert.h"

namespace RoseCommon {

SrvAcceptReply::SrvAcceptReply() : CRosePacket(ePacketType::PAKSS_ACCEPT_REPLY) {}

SrvAcceptReply::SrvAcceptReply(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKSS_ACCEPT_REPLY, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint8_t(result_);
	reader.get_uint32_t(randValue_);
}

SrvAcceptReply::SrvAcceptReply(uint8_t result, uint32_t randValue) : CRosePacket(ePacketType::PAKSS_ACCEPT_REPLY), result_(result), randValue_(randValue) {
}

uint8_t SrvAcceptReply::result() const { return result_; }

SrvAcceptReply& SrvAcceptReply::set_result(uint8_t data) { result_ = data; return *this; }

uint32_t SrvAcceptReply::randValue() const { return randValue_; }

SrvAcceptReply& SrvAcceptReply::set_randValue(uint32_t data) { randValue_ = data; return *this; }


void SrvAcceptReply::pack(CRoseBasePolicy& writer) const {
	writer.set_uint8_t(result_);
	writer.set_uint32_t(randValue_);
}

SrvAcceptReply SrvAcceptReply::create(uint8_t result, uint32_t randValue) {


	return SrvAcceptReply(result, randValue);
}

SrvAcceptReply SrvAcceptReply::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvAcceptReply(reader);
}
std::unique_ptr<SrvAcceptReply> SrvAcceptReply::allocate(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return std::make_unique<SrvAcceptReply>(reader);
}

}
#include "srv_deletecharreply.h"
#include "throwassert.h"

namespace RoseCommon {

SrvDeleteCharReply::SrvDeleteCharReply() : CRosePacket(ePacketType::PAKCC_DELETE_CHAR_REPLY) {}

SrvDeleteCharReply::SrvDeleteCharReply(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKCC_DELETE_CHAR_REPLY, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint32_t(remainingTime_);
	reader.get_string(name_);
}

SrvDeleteCharReply::SrvDeleteCharReply(uint32_t remainingTime, std::string name) : CRosePacket(ePacketType::PAKCC_DELETE_CHAR_REPLY), remainingTime_(remainingTime), name_(name) {
}

uint32_t SrvDeleteCharReply::remainingTime() const { return remainingTime_; }

SrvDeleteCharReply& SrvDeleteCharReply::set_remainingTime(uint32_t data) { remainingTime_ = data; return *this; }

std::string SrvDeleteCharReply::name() const { return name_; }

SrvDeleteCharReply& SrvDeleteCharReply::set_name(std::string data) { name_ = data; return *this; }


void SrvDeleteCharReply::pack(CRoseBasePolicy& writer) const {
	writer.set_uint32_t(remainingTime_);
	writer.set_string(name_);
}

SrvDeleteCharReply SrvDeleteCharReply::create(uint32_t remainingTime, std::string name) {


	return SrvDeleteCharReply(remainingTime, name);
}

SrvDeleteCharReply SrvDeleteCharReply::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvDeleteCharReply(reader);
}
std::unique_ptr<SrvDeleteCharReply> SrvDeleteCharReply::allocate(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return std::make_unique<SrvDeleteCharReply>(reader);
}

}
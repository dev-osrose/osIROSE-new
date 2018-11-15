#include "srv_logoutreply.h"
#include "throwassert.h"

namespace RoseCommon {

SrvLogoutReply::SrvLogoutReply() : CRosePacket(ePacketType::PAKWC_LOGOUT_REPLY) {}

SrvLogoutReply::SrvLogoutReply(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKWC_LOGOUT_REPLY, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint16_t(waitTime_);
}

SrvLogoutReply::SrvLogoutReply(uint16_t waitTime) : CRosePacket(ePacketType::PAKWC_LOGOUT_REPLY), waitTime_(waitTime) {
}

uint16_t SrvLogoutReply::waitTime() const { return waitTime_; }

SrvLogoutReply& SrvLogoutReply::set_waitTime(uint16_t data) { waitTime_ = data; return *this; }


void SrvLogoutReply::pack(CRoseWriter& writer) const {
	writer.set_uint16_t(waitTime_);
}

uint16_t SrvLogoutReply::get_size() const {
	uint16_t size = 0;
	size += sizeof(waitTime_);
	return size;
}


SrvLogoutReply SrvLogoutReply::create(uint16_t waitTime) {


	return SrvLogoutReply(waitTime);
}

SrvLogoutReply SrvLogoutReply::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvLogoutReply(reader);
}

}
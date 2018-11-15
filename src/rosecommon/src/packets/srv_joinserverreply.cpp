#include "srv_joinserverreply.h"
#include "throwassert.h"

namespace RoseCommon {

SrvJoinServerReply::SrvJoinServerReply() : CRosePacket(ePacketType::PAKSC_JOIN_SERVER_REPLY) {}

SrvJoinServerReply::SrvJoinServerReply(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKSC_JOIN_SERVER_REPLY, "Not the right packet: " << to_underlying(get_type()));
	{ uint8_t _tmp = uint8_t(); reader.get_uint8_t(_tmp); result_ = static_cast<JoinServerReply::Result>(_tmp); }
	reader.get_uint32_t(id_);
	reader.get_uint32_t(payFlag_);
}

SrvJoinServerReply::SrvJoinServerReply(JoinServerReply::Result result, uint32_t id) : CRosePacket(ePacketType::PAKSC_JOIN_SERVER_REPLY), result_(result), id_(id) {
}

JoinServerReply::Result SrvJoinServerReply::result() const { return result_; }

SrvJoinServerReply& SrvJoinServerReply::set_result(JoinServerReply::Result data) { result_ = data; return *this; }

uint32_t SrvJoinServerReply::id() const { return id_; }

SrvJoinServerReply& SrvJoinServerReply::set_id(uint32_t data) { id_ = data; return *this; }

uint32_t SrvJoinServerReply::payFlag() const { return payFlag_; }

SrvJoinServerReply& SrvJoinServerReply::set_payFlag(uint32_t data) { payFlag_ = data; return *this; }


void SrvJoinServerReply::pack(CRoseWriter& writer) const {
	writer.set_uint8_t(result_);
	writer.set_uint32_t(id_);
	writer.set_uint32_t(payFlag_);
}

uint16_t SrvJoinServerReply::get_size() const {
	uint16_t size = 0;
	size += sizeof(result_);
	size += sizeof(id_);
	size += sizeof(payFlag_);
	return size;
}


SrvJoinServerReply SrvJoinServerReply::create(JoinServerReply::Result result, uint32_t id) {


	return SrvJoinServerReply(result, id);
}

SrvJoinServerReply SrvJoinServerReply::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvJoinServerReply(reader);
}

}
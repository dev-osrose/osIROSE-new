#include "srv_srvselectreply.h"
#include "throwassert.h"

namespace RoseCommon {

SrvSrvSelectReply::SrvSrvSelectReply() : CRosePacket(ePacketType::PAKLC_SRV_SELECT_REPLY) {}

SrvSrvSelectReply::SrvSrvSelectReply(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKLC_SRV_SELECT_REPLY, "Not the right packet: " << to_underlying(get_type()));
	{ uint8_t _tmp = uint8_t(); reader.get_uint8_t(_tmp); result_ = static_cast<SrvSelectReply::Result>(_tmp); }
	reader.get_uint32_t(sessionId_);
	reader.get_uint32_t(cryptVal_);
	reader.get_string(ip_);
	reader.get_uint16_t(port_);
}

SrvSrvSelectReply::SrvSrvSelectReply(SrvSelectReply::Result result, uint32_t sessionId, uint32_t cryptVal, const std::string& ip, uint16_t port) : CRosePacket(ePacketType::PAKLC_SRV_SELECT_REPLY), result_(result), sessionId_(sessionId), cryptVal_(cryptVal), ip_(ip), port_(port) {
}

SrvSelectReply::Result SrvSrvSelectReply::result() const { return result_; }

SrvSrvSelectReply& SrvSrvSelectReply::set_result(SrvSelectReply::Result data) { result_ = data; return *this; }

uint32_t SrvSrvSelectReply::sessionId() const { return sessionId_; }

SrvSrvSelectReply& SrvSrvSelectReply::set_sessionId(uint32_t data) { sessionId_ = data; return *this; }

uint32_t SrvSrvSelectReply::cryptVal() const { return cryptVal_; }

SrvSrvSelectReply& SrvSrvSelectReply::set_cryptVal(uint32_t data) { cryptVal_ = data; return *this; }

const std::string& SrvSrvSelectReply::ip() const { return ip_; }

SrvSrvSelectReply& SrvSrvSelectReply::set_ip(const std::string& data) { ip_ = data; return *this; }

uint16_t SrvSrvSelectReply::port() const { return port_; }

SrvSrvSelectReply& SrvSrvSelectReply::set_port(uint16_t data) { port_ = data; return *this; }


void SrvSrvSelectReply::pack(CRoseWriter& writer) const {
	writer.set_uint8_t(result_);
	writer.set_uint32_t(sessionId_);
	writer.set_uint32_t(cryptVal_);
	writer.set_string(ip_);
	writer.set_uint16_t(port_);
}

uint16_t SrvSrvSelectReply::get_size() const {
	uint16_t size = 0;
	size += sizeof(result_);
	size += sizeof(sessionId_);
	size += sizeof(cryptVal_);
	size += sizeof(char) * (ip_.size() + 1);
	size += sizeof(port_);
	return size;
}


SrvSrvSelectReply SrvSrvSelectReply::create(SrvSelectReply::Result result, uint32_t sessionId, uint32_t cryptVal, const std::string& ip, uint16_t port) {


	return SrvSrvSelectReply(result, sessionId, cryptVal, ip, port);
}

SrvSrvSelectReply SrvSrvSelectReply::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvSrvSelectReply(reader);
}

}
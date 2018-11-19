#include "cli_srvselectreq.h"
#include "throwassert.h"

namespace RoseCommon {

CliSrvSelectReq::CliSrvSelectReq() : CRosePacket(ePacketType::PAKCS_SRV_SELECT_REQ) {}

CliSrvSelectReq::CliSrvSelectReq(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKCS_SRV_SELECT_REQ, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint32_t(serverId_);
	reader.get_uint8_t(channelId_);
}

CliSrvSelectReq::CliSrvSelectReq(uint32_t serverId, uint8_t channelId) : CRosePacket(ePacketType::PAKCS_SRV_SELECT_REQ), serverId_(serverId), channelId_(channelId) {
}

uint32_t CliSrvSelectReq::serverId() const { return serverId_; }

CliSrvSelectReq& CliSrvSelectReq::set_serverId(uint32_t data) { serverId_ = data; return *this; }

uint8_t CliSrvSelectReq::channelId() const { return channelId_; }

CliSrvSelectReq& CliSrvSelectReq::set_channelId(uint8_t data) { channelId_ = data; return *this; }


void CliSrvSelectReq::pack(CRoseBasePolicy& writer) const {
	writer.set_uint32_t(serverId_);
	writer.set_uint8_t(channelId_);
}

CliSrvSelectReq CliSrvSelectReq::create(uint32_t serverId, uint8_t channelId) {


	return CliSrvSelectReq(serverId, channelId);
}

CliSrvSelectReq CliSrvSelectReq::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return CliSrvSelectReq(reader);
}
std::unique_ptr<CliSrvSelectReq> CliSrvSelectReq::allocate(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return std::make_unique<CliSrvSelectReq>(reader);
}

}
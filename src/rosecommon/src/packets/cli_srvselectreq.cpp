#include "cli_srvselectreq.h"
#include "throwassert.h"

namespace RoseCommon {

const RecvPacketFactory::Initializer<uint8_t[MAX_PACKET_SIZE]> CliSrvSelectReq::init = RecvPacketFactory::Initializer<uint8_t[MAX_PACKET_SIZE]>(ePacketType::PAKCS_SRV_SELECT_REQ, &createPacket<CliSrvSelectReq>);

CliSrvSelectReq::CliSrvSelectReq() : CRosePacket(ePacketType::PAKCS_SRV_SELECT_REQ) {}

CliSrvSelectReq::CliSrvSelectReq(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {
	throw_assert(type() == ePacketType::PAKCS_SRV_SELECT_REQ, "Not the right packet: " << to_underlying(type()));
	*this >> serverId_;
	*this >> channelId_;
}

CliSrvSelectReq::CliSrvSelectReq(uint32_t serverId, uint8_t channelId) : CRosePacket(ePacketType::PAKCS_SRV_SELECT_REQ), serverId_(serverId), channelId_(channelId) {}

uint32_t CliSrvSelectReq::serverId() const {
	return serverId_;
}

uint8_t CliSrvSelectReq::channelId() const {
	return channelId_;
}


void CliSrvSelectReq::pack() {
	*this << serverId_;
	*this << channelId_;
}

}
#include "cli_srv_select_req.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;



CliSrvSelectReq::CliSrvSelectReq() : CRosePacket(ePacketType::PAKCS_SRV_SELECT_REQ) {}

CliSrvSelectReq::CliSrvSelectReq(CRoseReader reader) : CRosePacket(reader) {
    if (!reader.get_uint32_t(serverId)) {
        return;
    }
    if (!reader.get_uint8_t(channelId)) {
        return;
    }
}

void CliSrvSelectReq::set_serverId(const uint32_t serverId) {
    this->serverId = serverId;
}

uint32_t CliSrvSelectReq::get_serverId() const {
    return serverId;
}

void CliSrvSelectReq::set_channelId(const uint8_t channelId) {
    this->channelId = channelId;
}

uint8_t CliSrvSelectReq::get_channelId() const {
    return channelId;
}

CliSrvSelectReq CliSrvSelectReq::create(const uint32_t& serverId, const uint8_t& channelId) {
    CliSrvSelectReq packet;
    packet.set_serverId(serverId);
    packet.set_channelId(channelId);
    return packet;
}

CliSrvSelectReq CliSrvSelectReq::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return CliSrvSelectReq(reader);
}

void CliSrvSelectReq::pack(CRoseBasePolicy& writer) const {
    if (!writer.set_uint32_t(serverId)) {
        return;
    }
    if (!writer.set_uint8_t(channelId)) {
        return;
    }
}

constexpr size_t CliSrvSelectReq::size() {
    size_t size = 0;
    size += sizeof(uint32_t);
    size += sizeof(uint8_t);
    return size;
}


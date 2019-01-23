#include "cli_channel_list_req.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;



CliChannelListReq::CliChannelListReq() : CRosePacket(CliChannelListReq::PACKET_ID) {}

CliChannelListReq::CliChannelListReq(CRoseReader reader) : CRosePacket(reader) {
    if (!reader.get_uint32_t(serverId)) {
        return;
    }
}

void CliChannelListReq::set_serverId(const uint32_t serverId) {
    this->serverId = serverId;
}

uint32_t CliChannelListReq::get_serverId() const {
    return serverId;
}

CliChannelListReq CliChannelListReq::create(const uint32_t& serverId) {
    CliChannelListReq packet;
    packet.set_serverId(serverId);
    return packet;
}

CliChannelListReq CliChannelListReq::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return CliChannelListReq(reader);
}

std::unique_ptr<CliChannelListReq> CliChannelListReq::allocate(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return std::make_unique<CliChannelListReq>(reader);
}

bool CliChannelListReq::pack(CRoseBasePolicy& writer) const {
    if (!writer.set_uint32_t(serverId)) {
        return false;
    }
    return true;
}

constexpr size_t CliChannelListReq::size() {
    size_t size = 0;
    size += sizeof(uint32_t); // serverId
    return size;
}


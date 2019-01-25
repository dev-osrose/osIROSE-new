#include "srv_join_server_reply.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;



SrvJoinServerReply::SrvJoinServerReply() : CRosePacket(SrvJoinServerReply::PACKET_ID) {}

SrvJoinServerReply::SrvJoinServerReply(CRoseReader reader) : CRosePacket(reader) {
    if (!reader.get_uint8_t((uint8_t&)result)) {
        return;
    }
    if (!reader.get_uint32_t(id)) {
        return;
    }
    if (!reader.get_uint32_t(payFlag)) {
        return;
    }
}

void SrvJoinServerReply::set_result(const SrvJoinServerReply::Result result) {
    this->result = result;
}

SrvJoinServerReply::Result SrvJoinServerReply::get_result() const {
    return result;
}

void SrvJoinServerReply::set_id(const uint32_t id) {
    this->id = id;
}

uint32_t SrvJoinServerReply::get_id() const {
    return id;
}

void SrvJoinServerReply::set_payFlag(const uint32_t payFlag) {
    this->payFlag = payFlag;
}

uint32_t SrvJoinServerReply::get_payFlag() const {
    return payFlag;
}

SrvJoinServerReply SrvJoinServerReply::create(const SrvJoinServerReply::Result& result, const uint32_t& id) {
    SrvJoinServerReply packet;
    packet.set_result(result);
    packet.set_id(id);
    return packet;
}

SrvJoinServerReply SrvJoinServerReply::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return SrvJoinServerReply(reader);
}

std::unique_ptr<SrvJoinServerReply> SrvJoinServerReply::allocate(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return std::make_unique<SrvJoinServerReply>(reader);
}

bool SrvJoinServerReply::pack(CRoseBasePolicy& writer) const {
    if (!writer.set_uint8_t(result)) {
        return false;
    }
    if (!writer.set_uint32_t(id)) {
        return false;
    }
    if (!writer.set_uint32_t(payFlag)) {
        return false;
    }
    return true;
}

constexpr size_t SrvJoinServerReply::size() {
    size_t size = 0;
    size += sizeof(Result); // result
    size += sizeof(uint32_t); // id
    size += sizeof(uint32_t); // payFlag
    return size;
}


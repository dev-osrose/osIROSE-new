#include "srv_accept_reply.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;



SrvAcceptReply::SrvAcceptReply() : CRosePacket(SrvAcceptReply::PACKET_ID) {}

SrvAcceptReply::SrvAcceptReply(CRoseReader reader) : CRosePacket(reader) {
    if (!reader.get_uint8_t((uint8_t&)result)) {
        return;
    }
    if (!reader.get_uint32_t(randValue)) {
        return;
    }
}

void SrvAcceptReply::set_result(const SrvAcceptReply::Result result) {
    this->result = result;
}

SrvAcceptReply::Result SrvAcceptReply::get_result() const {
    return result;
}

void SrvAcceptReply::set_randValue(const uint32_t randValue) {
    this->randValue = randValue;
}

uint32_t SrvAcceptReply::get_randValue() const {
    return randValue;
}

SrvAcceptReply SrvAcceptReply::create(const SrvAcceptReply::Result& result, const uint32_t& randValue) {
    SrvAcceptReply packet;
    packet.set_result(result);
    packet.set_randValue(randValue);
    return packet;
}

SrvAcceptReply SrvAcceptReply::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return SrvAcceptReply(reader);
}

std::unique_ptr<SrvAcceptReply> SrvAcceptReply::allocate(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return std::make_unique<SrvAcceptReply>(reader);
}

void SrvAcceptReply::pack(CRoseBasePolicy& writer) const {
    if (!writer.set_uint8_t(result)) {
        return;
    }
    if (!writer.set_uint32_t(randValue)) {
        return;
    }
}

constexpr size_t SrvAcceptReply::size() {
    size_t size = 0;
    size += sizeof(Result); // result
    size += sizeof(uint32_t); // randValue
    return size;
}


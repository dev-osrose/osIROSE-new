#include "srv_delete_char_reply.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;



SrvDeleteCharReply::SrvDeleteCharReply() : CRosePacket(SrvDeleteCharReply::PACKET_ID) {}

SrvDeleteCharReply::SrvDeleteCharReply(CRoseReader reader) : CRosePacket(reader) {
    if (!reader.get_uint32_t(remainingTime)) {
        return;
    }
    if (!reader.get_string(name)) {
        return;
    }
}

void SrvDeleteCharReply::set_remainingTime(const uint32_t remainingTime) {
    this->remainingTime = remainingTime;
}

uint32_t SrvDeleteCharReply::get_remainingTime() const {
    return remainingTime;
}

void SrvDeleteCharReply::set_name(const std::string& name) {
    this->name = name;
}

const std::string& SrvDeleteCharReply::get_name() const {
    return name;
}

SrvDeleteCharReply SrvDeleteCharReply::create(const uint32_t& remainingTime, const std::string& name) {
    SrvDeleteCharReply packet;
    packet.set_remainingTime(remainingTime);
    packet.set_name(name);
    return packet;
}

SrvDeleteCharReply SrvDeleteCharReply::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return SrvDeleteCharReply(reader);
}

std::unique_ptr<SrvDeleteCharReply> SrvDeleteCharReply::allocate(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return std::make_unique<SrvDeleteCharReply>(reader);
}

void SrvDeleteCharReply::pack(CRoseBasePolicy& writer) const {
    if (!writer.set_uint32_t(remainingTime)) {
        return;
    }
    if (!writer.set_string(name)) {
        return;
    }
}

constexpr size_t SrvDeleteCharReply::size() {
    size_t size = 0;
    size += sizeof(uint32_t); // remainingTime
    return size;
}


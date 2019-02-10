#include "srv_party_reply.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;



SrvPartyReply::SrvPartyReply() : CRosePacket(SrvPartyReply::PACKET_ID) {}

SrvPartyReply::SrvPartyReply(CRoseReader reader) : CRosePacket(reader) {
    if (!reader.get_uint8_t((uint8_t&)reply)) {
        return;
    }
    if (!reader.get_uint32_t(tag)) {
        return;
    }
}

void SrvPartyReply::set_reply(const SrvPartyReply::Reply reply) {
    this->reply = reply;
}

SrvPartyReply::Reply SrvPartyReply::get_reply() const {
    return reply;
}

void SrvPartyReply::set_tag(const uint32_t tag) {
    this->tag = tag;
}

uint32_t SrvPartyReply::get_tag() const {
    return tag;
}

SrvPartyReply SrvPartyReply::create(const SrvPartyReply::Reply& reply, const uint32_t& tag) {
    SrvPartyReply packet;
    packet.set_reply(reply);
    packet.set_tag(tag);
    return packet;
}

SrvPartyReply SrvPartyReply::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return SrvPartyReply(reader);
}

std::unique_ptr<SrvPartyReply> SrvPartyReply::allocate(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return std::make_unique<SrvPartyReply>(reader);
}

bool SrvPartyReply::pack(CRoseBasePolicy& writer) const {
    if (!writer.set_uint8_t(reply)) {
        return false;
    }
    if (!writer.set_uint32_t(tag)) {
        return false;
    }
    return true;
}

constexpr size_t SrvPartyReply::size() {
    size_t size = 0;
    size += sizeof(Reply); // reply
    size += sizeof(uint32_t); // tag
    return size;
}


#include "cli_party_reply.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;



CliPartyReply::CliPartyReply() : CRosePacket(CliPartyReply::PACKET_ID) {}

CliPartyReply::CliPartyReply(CRoseReader reader) : CRosePacket(reader) {
    if (!reader.get_uint8_t((uint8_t&)reply)) {
        return;
    }
    if (!reader.get_uint32_t(idXorTag)) {
        return;
    }
}

void CliPartyReply::set_reply(const CliPartyReply::Reply reply) {
    this->reply = reply;
}

CliPartyReply::Reply CliPartyReply::get_reply() const {
    return reply;
}

void CliPartyReply::set_idXorTag(const uint32_t idXorTag) {
    this->idXorTag = idXorTag;
}

uint32_t CliPartyReply::get_idXorTag() const {
    return idXorTag;
}

CliPartyReply CliPartyReply::create(const CliPartyReply::Reply& reply, const uint32_t& idXorTag) {
    CliPartyReply packet;
    packet.set_reply(reply);
    packet.set_idXorTag(idXorTag);
    return packet;
}

CliPartyReply CliPartyReply::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return CliPartyReply(reader);
}

std::unique_ptr<CliPartyReply> CliPartyReply::allocate(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return std::make_unique<CliPartyReply>(reader);
}

bool CliPartyReply::pack(CRoseBasePolicy& writer) const {
    if (!writer.set_uint8_t(reply)) {
        return false;
    }
    if (!writer.set_uint32_t(idXorTag)) {
        return false;
    }
    return true;
}

constexpr size_t CliPartyReply::size() {
    size_t size = 0;
    size += sizeof(Reply); // reply
    size += sizeof(uint32_t); // idXorTag
    return size;
}


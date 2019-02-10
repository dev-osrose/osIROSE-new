#include "srv_party_req.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;



SrvPartyReq::SrvPartyReq() : CRosePacket(SrvPartyReq::PACKET_ID) {}

SrvPartyReq::SrvPartyReq(CRoseReader reader) : CRosePacket(reader) {
    if (!reader.get_uint8_t((uint8_t&)request)) {
        return;
    }
    if (!reader.get_uint32_t(tag)) {
        return;
    }
    if (!reader.get_string(name)) {
        return;
    }
}

void SrvPartyReq::set_request(const SrvPartyReq::Request request) {
    this->request = request;
}

SrvPartyReq::Request SrvPartyReq::get_request() const {
    return request;
}

void SrvPartyReq::set_tag(const uint32_t tag) {
    this->tag = tag;
}

uint32_t SrvPartyReq::get_tag() const {
    return tag;
}

void SrvPartyReq::set_name(const std::string& name) {
    this->name = name;
}

const std::string& SrvPartyReq::get_name() const {
    return name;
}

SrvPartyReq SrvPartyReq::create(const SrvPartyReq::Request& request, const uint32_t& tag, const std::string& name) {
    SrvPartyReq packet;
    packet.set_request(request);
    packet.set_tag(tag);
    packet.set_name(name);
    return packet;
}

SrvPartyReq SrvPartyReq::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return SrvPartyReq(reader);
}

std::unique_ptr<SrvPartyReq> SrvPartyReq::allocate(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return std::make_unique<SrvPartyReq>(reader);
}

bool SrvPartyReq::pack(CRoseBasePolicy& writer) const {
    if (!writer.set_uint8_t(request)) {
        return false;
    }
    if (!writer.set_uint32_t(tag)) {
        return false;
    }
    if (!writer.set_string(name)) {
        return false;
    }
    return true;
}

constexpr size_t SrvPartyReq::size() {
    size_t size = 0;
    size += sizeof(Request); // request
    size += sizeof(uint32_t); // tag
    return size;
}


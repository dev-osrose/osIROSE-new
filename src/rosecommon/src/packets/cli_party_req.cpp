#include "cli_party_req.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;



CliPartyReq::CliPartyReq() : CRosePacket(CliPartyReq::PACKET_ID) {}

CliPartyReq::CliPartyReq(CRoseReader reader) : CRosePacket(reader) {
    if (!reader.get_uint8_t((uint8_t&)request)) {
        return;
    }
    if (!reader.get_uint32_t(idXorTag)) {
        return;
    }
}

void CliPartyReq::set_request(const CliPartyReq::Request request) {
    this->request = request;
}

CliPartyReq::Request CliPartyReq::get_request() const {
    return request;
}

void CliPartyReq::set_idXorTag(const uint32_t idXorTag) {
    this->idXorTag = idXorTag;
}

uint32_t CliPartyReq::get_idXorTag() const {
    return idXorTag;
}

CliPartyReq CliPartyReq::create(const CliPartyReq::Request& request, const uint32_t& idXorTag) {
    CliPartyReq packet;
    packet.set_request(request);
    packet.set_idXorTag(idXorTag);
    return packet;
}

CliPartyReq CliPartyReq::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return CliPartyReq(reader);
}

std::unique_ptr<CliPartyReq> CliPartyReq::allocate(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return std::make_unique<CliPartyReq>(reader);
}

bool CliPartyReq::pack(CRoseBasePolicy& writer) const {
    if (!writer.set_uint8_t(request)) {
        return false;
    }
    if (!writer.set_uint32_t(idXorTag)) {
        return false;
    }
    return true;
}

constexpr size_t CliPartyReq::size() {
    size_t size = 0;
    size += sizeof(Request); // request
    size += sizeof(uint32_t); // idXorTag
    return size;
}


#include "cli_accept_req.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;



CliAcceptReq::CliAcceptReq() : CRosePacket(CliAcceptReq::PACKET_ID) {}

CliAcceptReq::CliAcceptReq(CRoseReader reader) : CRosePacket(reader) {
}

CliAcceptReq CliAcceptReq::create() {
    CliAcceptReq packet;
    return packet;
}

CliAcceptReq CliAcceptReq::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return CliAcceptReq(reader);
}

std::unique_ptr<CliAcceptReq> CliAcceptReq::allocate(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return std::make_unique<CliAcceptReq>(reader);
}

bool CliAcceptReq::pack(CRoseBasePolicy&) const {
    return true;
}

constexpr size_t CliAcceptReq::size() {
    size_t size = 0;
    return size;
}


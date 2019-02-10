#include "srv_party_member.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;



SrvPartyMember::SrvPartyMember() : CRosePacket(SrvPartyMember::PACKET_ID) {}

SrvPartyMember::SrvPartyMember(CRoseReader reader) : CRosePacket(reader) {
    if (!reader.get_uint8_t(rules)) {
        return;
    }
    if (!reader.get_PartyData(data)) {
        return;
    }
}

void SrvPartyMember::set_rules(const uint8_t rules) {
    this->rules = rules;
}

uint8_t SrvPartyMember::get_rules() const {
    return rules;
}

void SrvPartyMember::set_data(const RoseCommon::PartyData data) {
    this->data = data;
}

RoseCommon::PartyData SrvPartyMember::get_data() const {
    return data;
}

SrvPartyMember SrvPartyMember::create(const uint8_t& rules, const RoseCommon::PartyData& data) {
    SrvPartyMember packet;
    packet.set_rules(rules);
    packet.set_data(data);
    return packet;
}

SrvPartyMember SrvPartyMember::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return SrvPartyMember(reader);
}

std::unique_ptr<SrvPartyMember> SrvPartyMember::allocate(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return std::make_unique<SrvPartyMember>(reader);
}

bool SrvPartyMember::pack(CRoseBasePolicy& writer) const {
    if (!writer.set_uint8_t(rules)) {
        return false;
    }
    if (!writer.set_PartyData(data)) {
        return false;
    }
    return true;
}

constexpr size_t SrvPartyMember::size() {
    size_t size = 0;
    size += sizeof(uint8_t); // rules
    size += sizeof(RoseCommon::PartyData); // data
    return size;
}


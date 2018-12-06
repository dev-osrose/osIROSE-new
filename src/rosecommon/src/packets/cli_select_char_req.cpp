#include "cli_select_char_req.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;



CliSelectCharReq::CliSelectCharReq() : CRosePacket(ePacketType::PAKCS_SELECT_CHAR_REQ) {}

CliSelectCharReq::CliSelectCharReq(CRoseReader reader) : CRosePacket(reader) {
    if (!reader.get_uint8_t(charId)) {
        return;
    }
    if (!reader.get_uint8_t(runMode)) {
        return;
    }
    if (!reader.get_uint8_t(rideMode)) {
        return;
    }
    if (!reader.get_string(name)) {
        return;
    }
}

void CliSelectCharReq::set_charId(const uint8_t charId) {
    this->charId = charId;
}

uint8_t CliSelectCharReq::get_charId() const {
    return charId;
}

void CliSelectCharReq::set_runMode(const uint8_t runMode) {
    this->runMode = runMode;
}

uint8_t CliSelectCharReq::get_runMode() const {
    return runMode;
}

void CliSelectCharReq::set_rideMode(const uint8_t rideMode) {
    this->rideMode = rideMode;
}

uint8_t CliSelectCharReq::get_rideMode() const {
    return rideMode;
}

void CliSelectCharReq::set_name(const std::string& name) {
    this->name = name;
}

const std::string& CliSelectCharReq::get_name() const {
    return name;
}

CliSelectCharReq CliSelectCharReq::create(const uint8_t& charId, const uint8_t& runMode, const uint8_t& rideMode, const std::string& name) {
    CliSelectCharReq packet;
    packet.set_charId(charId);
    packet.set_runMode(runMode);
    packet.set_rideMode(rideMode);
    packet.set_name(name);
    return packet;
}

CliSelectCharReq CliSelectCharReq::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return CliSelectCharReq(reader);
}

void CliSelectCharReq::pack(CRoseBasePolicy& writer) const {
    if (!writer.set_uint8_t(charId)) {
        return;
    }
    if (!writer.set_uint8_t(runMode)) {
        return;
    }
    if (!writer.set_uint8_t(rideMode)) {
        return;
    }
    if (!writer.set_string(name)) {
        return;
    }
}

constexpr size_t CliSelectCharReq::size() {
    size_t size = 0;
    size += sizeof(uint8_t);
    size += sizeof(uint8_t);
    size += sizeof(uint8_t);
    return size;
}


#include "cli_create_char_req.h"

using namespace RoseCommon;
using namespace RoseCommon::Packet;



CliCreateCharReq::CliCreateCharReq() : CRosePacket(ePacketType::PAKCS_CREATE_CHAR_REQ) {}

CliCreateCharReq::CliCreateCharReq(CRoseReader reader) : CRosePacket(reader) {
    if (!reader.get_uint8_t(race)) {
        return;
    }
    if (!reader.get_uint8_t(stone)) {
        return;
    }
    if (!reader.get_uint8_t(hair)) {
        return;
    }
    if (!reader.get_uint8_t(face)) {
        return;
    }
    if (!reader.get_uint8_t(weapon)) {
        return;
    }
    if (!reader.get_uint16_t(zone)) {
        return;
    }
    if (!reader.get_string(name)) {
        return;
    }
}

void CliCreateCharReq::set_race(const uint8_t race) {
    this->race = race;
}

uint8_t CliCreateCharReq::get_race() const {
    return race;
}

void CliCreateCharReq::set_stone(const uint8_t stone) {
    this->stone = stone;
}

uint8_t CliCreateCharReq::get_stone() const {
    return stone;
}

void CliCreateCharReq::set_hair(const uint8_t hair) {
    this->hair = hair;
}

uint8_t CliCreateCharReq::get_hair() const {
    return hair;
}

void CliCreateCharReq::set_face(const uint8_t face) {
    this->face = face;
}

uint8_t CliCreateCharReq::get_face() const {
    return face;
}

void CliCreateCharReq::set_weapon(const uint8_t weapon) {
    this->weapon = weapon;
}

uint8_t CliCreateCharReq::get_weapon() const {
    return weapon;
}

void CliCreateCharReq::set_zone(const uint16_t zone) {
    this->zone = zone;
}

uint16_t CliCreateCharReq::get_zone() const {
    return zone;
}

void CliCreateCharReq::set_name(const std::string& name) {
    this->name = name;
}

const std::string& CliCreateCharReq::get_name() const {
    return name;
}

CliCreateCharReq CliCreateCharReq::create(const uint8_t& race, const uint8_t& stone, const uint8_t& hair, const uint8_t& face, const uint8_t& weapon, const uint16_t& zone, const std::string& name) {
    CliCreateCharReq packet;
    packet.set_race(race);
    packet.set_stone(stone);
    packet.set_hair(hair);
    packet.set_face(face);
    packet.set_weapon(weapon);
    packet.set_zone(zone);
    packet.set_name(name);
    return packet;
}

CliCreateCharReq CliCreateCharReq::create(const uint8_t* buffer) {
    CRoseReader reader(buffer, CRosePacket::size(buffer));
    return CliCreateCharReq(reader);
}

void CliCreateCharReq::pack(CRoseBasePolicy& writer) const {
    if (!writer.set_uint8_t(race)) {
        return;
    }
    if (!writer.set_uint8_t(stone)) {
        return;
    }
    if (!writer.set_uint8_t(hair)) {
        return;
    }
    if (!writer.set_uint8_t(face)) {
        return;
    }
    if (!writer.set_uint8_t(weapon)) {
        return;
    }
    if (!writer.set_uint16_t(zone)) {
        return;
    }
    if (!writer.set_string(name)) {
        return;
    }
}

constexpr size_t CliCreateCharReq::size() {
    size_t size = 0;
    size += sizeof(uint8_t);
    size += sizeof(uint8_t);
    size += sizeof(uint8_t);
    size += sizeof(uint8_t);
    size += sizeof(uint8_t);
    size += sizeof(uint16_t);
    return size;
}


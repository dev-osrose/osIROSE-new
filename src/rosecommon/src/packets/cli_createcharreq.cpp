#include "cli_createcharreq.h"
#include "throwassert.h"

namespace RoseCommon {

CliCreateCharReq::CliCreateCharReq() : CRosePacket(ePacketType::PAKCS_CREATE_CHAR_REQ) {}

CliCreateCharReq::CliCreateCharReq(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKCS_CREATE_CHAR_REQ, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint8_t(race_);
	reader.get_uint8_t(stone_);
	reader.get_uint8_t(hair_);
	reader.get_uint8_t(face_);
	reader.get_uint8_t(weapon_);
	reader.get_uint16_t(zone_);
	reader.get_string(name_);
}

CliCreateCharReq::CliCreateCharReq(uint8_t race, uint8_t stone, uint8_t hair, uint8_t face, uint8_t weapon, uint16_t zone, const std::string& name) : CRosePacket(ePacketType::PAKCS_CREATE_CHAR_REQ), race_(race), stone_(stone), hair_(hair), face_(face), weapon_(weapon), zone_(zone), name_(name) {
}

uint8_t CliCreateCharReq::race() const { return race_; }

CliCreateCharReq& CliCreateCharReq::set_race(uint8_t data) { race_ = data; return *this; }

uint8_t CliCreateCharReq::stone() const { return stone_; }

CliCreateCharReq& CliCreateCharReq::set_stone(uint8_t data) { stone_ = data; return *this; }

uint8_t CliCreateCharReq::hair() const { return hair_; }

CliCreateCharReq& CliCreateCharReq::set_hair(uint8_t data) { hair_ = data; return *this; }

uint8_t CliCreateCharReq::face() const { return face_; }

CliCreateCharReq& CliCreateCharReq::set_face(uint8_t data) { face_ = data; return *this; }

uint8_t CliCreateCharReq::weapon() const { return weapon_; }

CliCreateCharReq& CliCreateCharReq::set_weapon(uint8_t data) { weapon_ = data; return *this; }

uint16_t CliCreateCharReq::zone() const { return zone_; }

CliCreateCharReq& CliCreateCharReq::set_zone(uint16_t data) { zone_ = data; return *this; }

const std::string& CliCreateCharReq::name() const { return name_; }

CliCreateCharReq& CliCreateCharReq::set_name(const std::string& data) { name_ = data; return *this; }


void CliCreateCharReq::pack(CRoseBasePolicy& writer) const {
	writer.set_uint8_t(race_);
	writer.set_uint8_t(stone_);
	writer.set_uint8_t(hair_);
	writer.set_uint8_t(face_);
	writer.set_uint8_t(weapon_);
	writer.set_uint16_t(zone_);
	writer.set_string(name_);
}

CliCreateCharReq CliCreateCharReq::create(uint8_t race, uint8_t stone, uint8_t hair, uint8_t face, uint8_t weapon, uint16_t zone, const std::string& name) {


	return CliCreateCharReq(race, stone, hair, face, weapon, zone, name);
}

CliCreateCharReq CliCreateCharReq::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return CliCreateCharReq(reader);
}
std::unique_ptr<CliCreateCharReq> CliCreateCharReq::allocate(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return std::make_unique<CliCreateCharReq>(reader);
}

}
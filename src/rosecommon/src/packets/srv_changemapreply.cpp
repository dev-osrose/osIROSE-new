#include "srv_changemapreply.h"
#include "throwassert.h"

namespace RoseCommon {

SrvChangeMapReply::SrvChangeMapReply() : CRosePacket(ePacketType::PAKWC_CHANGE_MAP_REPLY) {}

SrvChangeMapReply::SrvChangeMapReply(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKWC_CHANGE_MAP_REPLY, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint16_t(objectIndex_);
	reader.get_uint16_t(currentHp_);
	reader.get_uint16_t(currentMp_);
	reader.get_uint16_t(currentExp_);
	reader.get_uint16_t(penalizeExp_);
	reader.get_uint16_t(craftRate_);
	reader.get_uint16_t(updateTime_);
	reader.get_uint16_t(worldRate_);
	reader.get_uint8_t(townRate_);
	for (size_t index = 0; index < MAX_SELL_TYPE; ++index) reader.get_uint8_t(itemRate_[index]);
	reader.get_uint32_t(flags_);
	reader.get_uint16_t(worldTime_);
	reader.get_uint16_t(teamNumber_);
}

SrvChangeMapReply::SrvChangeMapReply(uint16_t objectIndex, uint16_t currentHp, uint16_t currentMp, uint16_t currentExp, uint16_t penalizeExp, uint16_t worldTime, uint16_t teamNumber) : CRosePacket(ePacketType::PAKWC_CHANGE_MAP_REPLY), objectIndex_(objectIndex), currentHp_(currentHp), currentMp_(currentMp), currentExp_(currentExp), penalizeExp_(penalizeExp), worldTime_(worldTime), teamNumber_(teamNumber) {
}

uint16_t SrvChangeMapReply::objectIndex() const { return objectIndex_; }

SrvChangeMapReply& SrvChangeMapReply::set_objectIndex(uint16_t data) { objectIndex_ = data; return *this; }

uint16_t SrvChangeMapReply::currentHp() const { return currentHp_; }

SrvChangeMapReply& SrvChangeMapReply::set_currentHp(uint16_t data) { currentHp_ = data; return *this; }

uint16_t SrvChangeMapReply::currentMp() const { return currentMp_; }

SrvChangeMapReply& SrvChangeMapReply::set_currentMp(uint16_t data) { currentMp_ = data; return *this; }

uint16_t SrvChangeMapReply::currentExp() const { return currentExp_; }

SrvChangeMapReply& SrvChangeMapReply::set_currentExp(uint16_t data) { currentExp_ = data; return *this; }

uint16_t SrvChangeMapReply::penalizeExp() const { return penalizeExp_; }

SrvChangeMapReply& SrvChangeMapReply::set_penalizeExp(uint16_t data) { penalizeExp_ = data; return *this; }

uint16_t SrvChangeMapReply::craftRate() const { return craftRate_; }

SrvChangeMapReply& SrvChangeMapReply::set_craftRate(uint16_t data) { craftRate_ = data; return *this; }

uint16_t SrvChangeMapReply::updateTime() const { return updateTime_; }

SrvChangeMapReply& SrvChangeMapReply::set_updateTime(uint16_t data) { updateTime_ = data; return *this; }

uint16_t SrvChangeMapReply::worldRate() const { return worldRate_; }

SrvChangeMapReply& SrvChangeMapReply::set_worldRate(uint16_t data) { worldRate_ = data; return *this; }

uint8_t SrvChangeMapReply::townRate() const { return townRate_; }

SrvChangeMapReply& SrvChangeMapReply::set_townRate(uint8_t data) { townRate_ = data; return *this; }

const uint8_t* SrvChangeMapReply::itemRate() const { return itemRate_; }

uint8_t SrvChangeMapReply::itemRate(size_t index) const { return itemRate_[index]; }

SrvChangeMapReply& SrvChangeMapReply::set_itemRate(const uint8_t* data) { for (size_t index = 0; index < MAX_SELL_TYPE; ++index) itemRate_[index] = data[index]; return *this; }

SrvChangeMapReply& SrvChangeMapReply::set_itemRate(uint8_t data, size_t index) { itemRate_[index] = data; return *this; }

uint32_t SrvChangeMapReply::flags() const { return flags_; }

SrvChangeMapReply& SrvChangeMapReply::set_flags(uint32_t data) { flags_ = data; return *this; }

uint16_t SrvChangeMapReply::worldTime() const { return worldTime_; }

SrvChangeMapReply& SrvChangeMapReply::set_worldTime(uint16_t data) { worldTime_ = data; return *this; }

uint16_t SrvChangeMapReply::teamNumber() const { return teamNumber_; }

SrvChangeMapReply& SrvChangeMapReply::set_teamNumber(uint16_t data) { teamNumber_ = data; return *this; }


void SrvChangeMapReply::pack(CRoseBasePolicy& writer) const {
	writer.set_uint16_t(objectIndex_);
	writer.set_uint16_t(currentHp_);
	writer.set_uint16_t(currentMp_);
	writer.set_uint16_t(currentExp_);
	writer.set_uint16_t(penalizeExp_);
	writer.set_uint16_t(craftRate_);
	writer.set_uint16_t(updateTime_);
	writer.set_uint16_t(worldRate_);
	writer.set_uint8_t(townRate_);
	for (size_t index = 0; index < MAX_SELL_TYPE; ++index) writer.set_uint8_t(itemRate_[index]);
	writer.set_uint32_t(flags_);
	writer.set_uint16_t(worldTime_);
	writer.set_uint16_t(teamNumber_);
}

SrvChangeMapReply SrvChangeMapReply::create(uint16_t objectIndex, uint16_t currentHp, uint16_t currentMp, uint16_t currentExp, uint16_t penalizeExp, uint16_t worldTime, uint16_t teamNumber) {


	return SrvChangeMapReply(objectIndex, currentHp, currentMp, currentExp, penalizeExp, worldTime, teamNumber);
}

SrvChangeMapReply SrvChangeMapReply::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvChangeMapReply(reader);
}
std::unique_ptr<SrvChangeMapReply> SrvChangeMapReply::allocate(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return std::make_unique<SrvChangeMapReply>(reader);
}

}
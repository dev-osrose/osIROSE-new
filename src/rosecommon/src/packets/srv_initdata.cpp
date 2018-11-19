#include "srv_initdata.h"
#include "throwassert.h"

namespace RoseCommon {

SrvInitData::SrvInitData() : CRosePacket(ePacketType::PAKWC_INIT_DATA) {}

SrvInitData::SrvInitData(CRoseReader reader) : CRosePacket(reader) {
	throw_assert(get_type() == ePacketType::PAKWC_INIT_DATA, "Not the right packet: " << to_underlying(get_type()));
	reader.get_uint32_t(randSeed_);
	reader.get_uint16_t(randIndex_);
}

SrvInitData::SrvInitData(uint32_t randSeed, uint16_t randIndex) : CRosePacket(ePacketType::PAKWC_INIT_DATA), randSeed_(randSeed), randIndex_(randIndex) {
}

uint32_t SrvInitData::randSeed() const { return randSeed_; }

SrvInitData& SrvInitData::set_randSeed(uint32_t data) { randSeed_ = data; return *this; }

uint16_t SrvInitData::randIndex() const { return randIndex_; }

SrvInitData& SrvInitData::set_randIndex(uint16_t data) { randIndex_ = data; return *this; }


void SrvInitData::pack(CRoseBasePolicy& writer) const {
	writer.set_uint32_t(randSeed_);
	writer.set_uint16_t(randIndex_);
}

SrvInitData SrvInitData::create(uint32_t randSeed, uint16_t randIndex) {


	return SrvInitData(randSeed, randIndex);
}

SrvInitData SrvInitData::create(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return SrvInitData(reader);
}
std::unique_ptr<SrvInitData> SrvInitData::allocate(uint8_t *buffer) {
	CRoseReader reader(buffer, CRosePacket::size(buffer));
	return std::make_unique<SrvInitData>(reader);
}

}
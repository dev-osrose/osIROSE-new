#include "srv_initdata.h"

namespace RoseCommon {

SrvInitData::SrvInitData() : CRosePacket(ePacketType::PAKWC_INIT_DATA) {}

SrvInitData::SrvInitData(uint32_t randSeed, uint16_t randIndex) : CRosePacket(ePacketType::PAKWC_INIT_DATA), randSeed_(randSeed), randIndex_(randIndex) {}

uint32_t SrvInitData::randSeed() const {
	return randSeed_;
}

uint16_t SrvInitData::randIndex() const {
	return randIndex_;
}


void SrvInitData::pack() {
	*this << randSeed_;
	*this << randIndex_;
}

}
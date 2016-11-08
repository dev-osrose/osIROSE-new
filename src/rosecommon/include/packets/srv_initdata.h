#pragma once

#include "packetfactory.h"
#include "entityComponents.h"

namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKWC_INIT_DATA, SrvInitData)
class SrvInitData : public CRosePacket {
	public:
		SrvInitData();

		SrvInitData(uint32_t randSeed, uint16_t randIndex);

		virtual ~SrvInitData() = default;

		uint32_t &randSeed();
		uint16_t &randIndex();
	protected:
		virtual void pack() override;


	private:
		uint32_t randSeed_;
		uint16_t randIndex_;
};

}
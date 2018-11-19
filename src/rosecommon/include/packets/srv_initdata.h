#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKWC_INIT_DATA, SrvInitData)
REGISTER_SEND_PACKET(ePacketType::PAKWC_INIT_DATA, SrvInitData)
class SrvInitData : public CRosePacket {
	public:
		SrvInitData();
		SrvInitData(CRoseReader reader);
	private:
		SrvInitData(uint32_t randSeed, uint16_t randIndex);
	public:

		virtual ~SrvInitData() = default;

		uint32_t randSeed() const;
		SrvInitData& set_randSeed(uint32_t);
		uint16_t randIndex() const;
		SrvInitData& set_randIndex(uint16_t);

		static SrvInitData create(uint32_t randSeed, uint16_t randIndex);
		static SrvInitData create(uint8_t *buffer);
		static std::unique_ptr<SrvInitData> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		uint32_t randSeed_;
		uint16_t randIndex_;
};

}
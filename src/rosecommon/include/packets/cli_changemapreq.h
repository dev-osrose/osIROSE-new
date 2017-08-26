#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_CHANGE_MAP_REQ, CliChangeMapReq)
class CliChangeMapReq : public CRosePacket {
	private:
		static const RecvPacketFactory::Initializer<uint8_t*> init;
	public:
		CliChangeMapReq();
		CliChangeMapReq(uint8_t buffer[MAX_PACKET_SIZE]);
		CliChangeMapReq(uint8_t weightRate, uint16_t z);

		virtual ~CliChangeMapReq() = default;

		uint8_t weightRate() const;
		uint16_t z() const;

	protected:
		virtual void pack() override;

	private:
		uint8_t weightRate_;
		uint16_t z_;
};

}

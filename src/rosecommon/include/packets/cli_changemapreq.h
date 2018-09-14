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
		void weightRate(uint8_t rate);
		uint16_t z() const;
		void z(uint16_t z);

	protected:
		virtual void pack() override;

	private:
		uint8_t weightRate_;
		uint16_t z_;
};

}

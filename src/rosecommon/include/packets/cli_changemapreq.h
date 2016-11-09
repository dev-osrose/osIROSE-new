#pragma once

#include "packetfactory.h"
#include "entityComponents.h"

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_CHANGE_MAP_REQ, CliChangeMapReq)
class CliChangeMapReq : public CRosePacket, public RegisterRecvPacket<ePacketType::PAKCS_CHANGE_MAP_REQ, CliChangeMapReq> {
	public:
		CliChangeMapReq(uint8_t buffer[MAX_PACKET_SIZE]);

		virtual ~CliChangeMapReq() = default;

		uint8_t &weightRate();
		uint16_t &z();

	private:
		uint8_t weightRate_;
		uint16_t z_;
};

}

#pragma once

#include "packetfactory.h"
#include "entityComponents.h"
#include <string>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_SELECT_CHAR_REQ, CliSelectCharReq)
class CliSelectCharReq : public CRosePacket, public RegisterRecvPacket<ePacketType::PAKCS_SELECT_CHAR_REQ, CliSelectCharReq> {
	public:
		CliSelectCharReq(uint8_t buffer[MAX_PACKET_SIZE]);
        CliSelectCharReq(uint8_t charId, uint8_t runMode, uint8_t rideMode, const std::string &name);

		virtual ~CliSelectCharReq() = default;

		uint8_t &charId();
		uint8_t &runMode();
		uint8_t &rideMode();
		std::string &name();

	private:
		uint8_t charId_;
		uint8_t runMode_;
		uint8_t rideMode_;
		std::string name_;
};

}

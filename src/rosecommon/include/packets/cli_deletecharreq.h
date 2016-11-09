#pragma once

#include "packetfactory.h"
#include "entityComponents.h"
#include <string>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_DELETE_CHAR_REQ, CliDeleteCharReq)
class CliDeleteCharReq : public CRosePacket, public RegisterRecvPacket<ePacketType::PAKCS_DELETE_CHAR_REQ, CliDeleteCharReq> {
	public:
		CliDeleteCharReq(uint8_t buffer[MAX_PACKET_SIZE]);

		virtual ~CliDeleteCharReq() = default;

		uint8_t &charId();
		bool isDelete() const;
		std::string &name();

	private:
		uint8_t charId_;
		uint8_t delete_;
		std::string name_;
};

}

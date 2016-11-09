#pragma once

#include "packetfactory.h"
#include "entityComponents.h"

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_REVIVE_REQ, CliReviveReq)
class CliReviveReq : public CRosePacket, public RegisterRecvPacket<ePacketType::PAKCS_REVIVE_REQ, CliReviveReq> {
	public:
		CliReviveReq(uint8_t buffer[MAX_PACKET_SIZE]);

		virtual ~CliReviveReq() = default;

        enum eType : uint8_t {
            REVIVE_POS,
            SAVE_POS,
            START_POS,
            CURRENT_POS
        };

		eType &type();

	private:
		eType type_;
};

}

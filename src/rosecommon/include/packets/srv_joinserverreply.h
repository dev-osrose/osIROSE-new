#pragma once

#include "packetfactory.h"
#include "entityComponents.h"


namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKSC_JOIN_SERVER_REPLY, SrvJoinServerReply)
class SrvJoinServerReply : public CRosePacket {
	public:
        enum eResult {
            OK = 0,
            FAILED,
            TIME_OUT,
            INVALID_PASSWORD,
            ALREADY_LOGGEDIN
        };

		SrvJoinServerReply();

		SrvJoinServerReply(eResult result, uint32_t id, uint32_t payFlag = 0);

		virtual ~SrvJoinServerReply() = default;

		eResult &result();
		uint32_t &id();
		uint32_t &payFlag();

	protected:
		virtual void pack() override;

	private:
		eResult result_;
		uint32_t id_;
		uint32_t payFlag_;
};

}

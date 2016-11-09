#pragma once

#include "packetfactory.h"
#include "entityComponents.h"
#include <string>

namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKLC_SRV_SELECT_REPLY, SrvSrvSelectReply)
class SrvSrvSelectReply : public CRosePacket {
	public:
        enum eResult : uint8_t {
            OK = 0,
            FAILED,
            FULL,
            INVALID_CHANNEL,
            CHANNEL_NOT_ACTIVE,
            INVALID_AGE
        };

		SrvSrvSelectReply();

		SrvSrvSelectReply(uint32_t sessionId, uint32_t cryptVal, uint16_t port, eResult result, std::string ip);

		virtual ~SrvSrvSelectReply() = default;

		uint32_t &sessionId();
		uint32_t &cryptVal();
		uint16_t &port();
		eResult &result();
		std::string &ip();
	protected:
		virtual void pack() override;


	private:
		uint32_t sessionId_;
		uint32_t cryptVal_;
		uint16_t port_;
		eResult result_;
		std::string ip_;
};

}

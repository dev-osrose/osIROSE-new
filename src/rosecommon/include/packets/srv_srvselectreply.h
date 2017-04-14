#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
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
		SrvSrvSelectReply(SrvSrvSelectReply::eResult result, uint32_t sessionId, uint32_t cryptVal, const std::string &ip, uint16_t port);

		virtual ~SrvSrvSelectReply() = default;

		SrvSrvSelectReply::eResult result() const;
		uint32_t sessionId() const;
		uint32_t cryptVal() const;
		std::string &ip();
		const std::string &ip() const;
		uint16_t port() const;

	protected:
		virtual void pack() override;

	private:
		SrvSrvSelectReply::eResult result_;
		uint32_t sessionId_;
		uint32_t cryptVal_;
		std::string ip_;
		uint16_t port_;
};

}

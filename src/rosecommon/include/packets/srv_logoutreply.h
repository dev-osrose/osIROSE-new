#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_SEND_PACKET(ePacketType::PAKWC_LOGOUT_REPLY, SrvLogoutReply)
class SrvLogoutReply : public CRosePacket {
	public:
		SrvLogoutReply();
		SrvLogoutReply(uint16_t waitTime);

		virtual ~SrvLogoutReply() = default;

		uint16_t waitTime() const;

	protected:
		virtual void pack() override;

	private:
		uint16_t waitTime_;
};

}
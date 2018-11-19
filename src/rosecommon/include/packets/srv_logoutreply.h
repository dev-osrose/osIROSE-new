#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKWC_LOGOUT_REPLY, SrvLogoutReply)
REGISTER_SEND_PACKET(ePacketType::PAKWC_LOGOUT_REPLY, SrvLogoutReply)
class SrvLogoutReply : public CRosePacket {
	public:
		SrvLogoutReply();
		SrvLogoutReply(CRoseReader reader);
	private:
		SrvLogoutReply(uint16_t waitTime);
	public:

		virtual ~SrvLogoutReply() = default;

		uint16_t waitTime() const;
		SrvLogoutReply& set_waitTime(uint16_t);

		static SrvLogoutReply create(uint16_t waitTime);
		static SrvLogoutReply create(uint8_t *buffer);
		static std::unique_ptr<SrvLogoutReply> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		uint16_t waitTime_;
};

}
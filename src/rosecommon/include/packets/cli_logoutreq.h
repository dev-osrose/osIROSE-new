#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_LOGOUT_REQ, CliLogoutReq)
REGISTER_SEND_PACKET(ePacketType::PAKCS_LOGOUT_REQ, CliLogoutReq)
class CliLogoutReq : public CRosePacket {
	public:
		CliLogoutReq();
		CliLogoutReq(CRoseReader reader);
	public:

		virtual ~CliLogoutReq() = default;


		static CliLogoutReq create();
		static CliLogoutReq create(uint8_t *buffer);
		static std::unique_ptr<CliLogoutReq> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;
};

}
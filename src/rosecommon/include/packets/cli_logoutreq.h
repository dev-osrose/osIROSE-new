#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"


namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_LOGOUT_REQ, CliLogoutReq)
class CliLogoutReq : public CRosePacket {
	private:
		static const RecvPacketFactory::Initializer<uint8_t*> init;
	public:
		CliLogoutReq();
		CliLogoutReq(uint8_t buffer[MAX_PACKET_SIZE]);

		virtual ~CliLogoutReq() = default;


	protected:
		virtual void pack() override;
};

}

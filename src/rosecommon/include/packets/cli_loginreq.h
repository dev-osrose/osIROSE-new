#pragma once

#include "packetfactory.h"
#include "entityComponents.h"
#include <string>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_LOGIN_REQ, CliLoginReq)
class CliLoginReq : public CRosePacket, public RegisterRecvPacket<ePacketType::PAKCS_LOGIN_REQ, CliLoginReq> {
	public:
		CliLoginReq(uint8_t buffer[MAX_PACKET_SIZE]);
        CliLoginReq(const std::string &user, const std::string& password);

        virtual void pack() override;

		virtual ~CliLoginReq() = default;

		std::string &username();
		std::string &password();

	private:
		std::string username_;
		std::string password_;
};

}

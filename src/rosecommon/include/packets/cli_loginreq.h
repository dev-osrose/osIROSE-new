#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <string>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_LOGIN_REQ, CliLoginReq)
class CliLoginReq : public CRosePacket {
	public:
		CliLoginReq();
		CliLoginReq(uint8_t buffer[MAX_PACKET_SIZE]);
		CliLoginReq(const std::string &password, const std::string &username);

		virtual ~CliLoginReq() = default;

		std::string &password();
		const std::string &password() const;
		std::string &username();
		const std::string &username() const;

	protected:
		virtual void pack() override;

	private:
		std::string password_;
		std::string username_;
};

}
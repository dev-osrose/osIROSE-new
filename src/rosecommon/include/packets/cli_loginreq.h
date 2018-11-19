#pragma once

#include "packetfactory.h"
#include "entitycomponents.h"
#include <string>

namespace RoseCommon {

REGISTER_RECV_PACKET(ePacketType::PAKCS_LOGIN_REQ, CliLoginReq)
REGISTER_SEND_PACKET(ePacketType::PAKCS_LOGIN_REQ, CliLoginReq)
class CliLoginReq : public CRosePacket {
	public:
		CliLoginReq();
		CliLoginReq(CRoseReader reader);
	private:
		CliLoginReq(const std::string& password, const std::string& username);
	public:

		virtual ~CliLoginReq() = default;

		const std::string& password() const;
		CliLoginReq& set_password(const std::string&);
		const std::string& username() const;
		CliLoginReq& set_username(const std::string&);

		static CliLoginReq create(const std::string& password, const std::string& username);
		static CliLoginReq create(uint8_t *buffer);
		static std::unique_ptr<CliLoginReq> allocate(uint8_t *buffer);

	protected:
		virtual void pack(CRoseBasePolicy&) const override;

	private:
		std::string password_;
		std::string username_;
};

}